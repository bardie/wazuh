#!/usr/bin/env python

# Created by Wazuh, Inc. <info@wazuh.com>.
# This program is a free software; you can redistribute it and/or modify it under the terms of GPLv2

from wazuh.utils import cut_array, sort_array, search_array, md5, send_request
from wazuh.exception import WazuhException
from wazuh import common
import sqlite3
from datetime import datetime
from hashlib import sha512
from time import time, mktime
from os import path, listdir, rename, utime, environ, umask, stat
from subprocess import check_output
import requests
from shutil import rmtree
from io import BytesIO
from itertools import compress
from operator import itemgetter
from ast import literal_eval
import socket
import json
import threading
from sys import version
# import the C accelerated API of ElementTree
try:
    import xml.etree.cElementTree as ET
except ImportError:
    import xml.etree.ElementTree as ET

try:
    import logging
    logging.basicConfig(filename=common.ossec_path+'/logs/cluster.log',level=logging.DEBUG,
                        format='%(asctime)s %(levelname)s: %(message)s')
except:
    pass

is_py2 = version[0] == '2'
if is_py2:
    from Queue import Queue as queue
else:
    from queue import Queue as queue

CLUSTER_ITEMS = [
    {
        "file_name":"/etc/client.keys",
        "umask": 0o117, # Allowed Permissions rw-rw----
        "format":"plain",
        "type": "file",
        "write_mode": "atomic",
        "conditions": {
            "higher_remote_time": True,
            "different_md5": True,
            "larger_remote_size": True
        }
    },
    {
        "file_name":"/queue/agent-info",
        "umask": 0o117, # Allowed Permissions rw-rw----
        "format":"plain",
        "type": "directory",
        "write_mode": "atomic",
        "conditions": {
            "higher_remote_time": True,
            "different_md5": False,
            "larger_remote_size": False
            }
    },
    {
        "file_name":"/queue/agent-groups",
        "umask": 0o117, # Allowed Permissions rw-rw----
        "format":"plain",
        "type": "directory",
        "write_mode": "atomic",
        "conditions": {
            "higher_remote_time": True,
            "different_md5": False,
            "larger_remote_size": False
            }
    }
    # {"file_name":"/etc/ossec.conf", "format":"xml"},
]

import zipfile

try:
    import zlib
    compression = zipfile.ZIP_DEFLATED
except:
    compression = zipfile.ZIP_STORED


def get_file_info(filename):
    fullpath = common.ossec_path + filename
    
    if not path.isfile(fullpath):
        raise WazuhException(3000, "Could not open file {0}".format(filename))

    stat_obj = stat(fullpath)
    st_mtime = stat_obj.st_mtime
    st_size = stat_obj.st_size

    new_item = CLUSTER_ITEMS[0] if filename == CLUSTER_ITEMS[0]['file_name'] else CLUSTER_ITEMS[1]

    file_item = {
        "umask" : new_item['umask'],
        "format" : new_item['format'],
        "write_mode" : new_item['write_mode'],
        "conditions" : new_item['conditions'],

        "md5": md5(fullpath),
        "modification_time" : str(datetime.utcfromtimestamp(st_mtime)),
        "size" : st_size,
        'is_synced': st_mtime.is_integer()
    }

    return file_item


def compress_files(list_path, conditions={'/etc/client.keys'}):
    zipped_file = BytesIO()
    conds = json.dumps({c:get_file_info(c) for c in conditions & list_path})
    with zipfile.ZipFile(zipped_file, 'w') as zf:
        # write files
        for f in list_path:
            try:
                zf.write(filename = common.ossec_path + f, arcname = f, compress_type=compression)
            except Exception as e:
                raise WazuhException(3001, str(e))

        # write conditions
        try:
            zf.writestr("conditions.txt", conds, compression)
        except Exception as e:
            raise WazuhException(3001, str(e))

    return zipped_file.getvalue()

def read_config():
    # Get api/configuration/config.js content
    try:
        cluster_xml = ET.parse(common.ossec_conf).find('cluster')
        config_cluster={child.tag:child.text for child in cluster_xml}
        config_cluster['nodes'] = [child.text for child in
                                   cluster_xml.find('nodes')]

    except Exception as e:
        raise WazuhException(3000, str(e))

    return config_cluster


get_localhost_ips = lambda: check_output(['hostname', '--all-ip-addresses']).split(" ")[:-1]

def get_nodes():
    config_cluster = read_config()
    if not config_cluster:
        raise WazuhException(3000, "No config found")

    # list with all the ips the localhost has
    localhost_ips = get_localhost_ips()
    data = []

    for url in config_cluster["nodes"]:
        if not url in localhost_ips:
            error, response = send_request(host=url, port=config_cluster["port"],
                                data="node {0}".format('a'*(common.cluster_sync_msg_size - len("node "))))
            if error == 0:
                response = response['data']
        else:
            error = 0
            url = "localhost"
            response = get_node()

        if error:
            data.append({'error': response, 'status':'disconnected', 'url':url})
            continue

        data.append({'url':url, 'node':response['node'],
                     'status':'connected', 'cluster':response['cluster']})

    return {'items': data, 'totalItems': len(data)}


def get_node(name=None):
    data = {}
    if not name:
        config_cluster = read_config()

        if not config_cluster:
            raise WazuhException(3000, "No config found")

        data["node"] = config_cluster["node_name"]
        data["cluster"] = config_cluster["name"]

    return data


def get_files():
    # Expand directory
    expanded_items = []
    for item in CLUSTER_ITEMS:
        file_path = item['file_name']

        if item["type"] == "file":
            new_item = dict(item)
            new_item["path"] = file_path
            expanded_items.append(new_item)
        else:
            fullpath = common.ossec_path + file_path
            for entry in listdir(fullpath):
                new_item = dict(item)
                new_item["path"] = path.join(file_path, entry)
                expanded_items.append(new_item)

    final_items = {}
    for new_item in expanded_items:
        try:
            final_items[new_item['path']] = get_file_info(new_item['path'])
        except Exception as e:
            continue

    return final_items


def get_token():
    config_cluster = read_config()

    if not config_cluster:
        raise WazuhException(3000, "No config found")

    raw_key = config_cluster["key"]
    token = sha512(raw_key).hexdigest()
    return token


def _check_token(other_token):
    my_token = get_token()
    if my_token == other_token:
        return True
    else:
        return False


def _update_file(fullpath, new_content, umask_int=None, mtime=None, w_mode=None):
    # Set Timezone to epoch converter
    environ['TZ']='UTC'

    # Write
    if w_mode == "atomic":
        f_temp = '{0}.tmp.cluster'.format(fullpath)
    else:
        f_temp = '{0}'.format(fullpath)

    if umask_int:
        oldumask = umask(umask_int)

    dest_file = open(f_temp, "w")
    dest_file.write(new_content)

    if umask_int:
        umask(oldumask)

    dest_file.close()

    mtime_epoch = int(mktime(mtime.timetuple()))
    utime(f_temp, (mtime_epoch, mtime_epoch)) # (atime, mtime)

    # Atomic
    if w_mode == "atomic":
        rename(f_temp, fullpath)

def extract_zip(zip_bytes):
    zip_json = {}
    with zipfile.ZipFile(BytesIO(zip_bytes)) as zipf:
        zip_json = {name:{'data':zipf.open(name).read(),
                          'time':datetime(*zipf.getinfo(name).date_time)}
                    for name in zipf.namelist()}

    return receive_zip(zip_json)

def receive_zip(zip_file):
    logging.info("Receiving zip with {0} files".format(len(zip_file)))

    final_dict = {'error':[], 'updated': [], 'invalid': []}
    # before saving files to disk, check conditions
    conditions = json.loads(zip_file['conditions.txt']['data'])
    del zip_file['conditions.txt']

    for filename, conds in conditions.items():
        local_conds = get_file_info(filename)

        checked_conditions = filter(lambda x: conds['conditions'][x],
                                              conds['conditions'].keys())
        try:
            remote_file_time = datetime.strptime(conds["modification_time"],
                                                "%Y-%m-%d %H:%M:%S.%f")
        except:
            remote_file_time = datetime.strptime(conds["modification_time"],
                                                "%Y-%m-%d %H:%M:%S")
        try:
            local_file_time = datetime.strptime(local_conds["modification_time"],
                                                "%Y-%m-%d %H:%M:%S.%f")
        except:
            local_file_time = datetime.strptime(local_conds["modification_time"],
                                                "%Y-%m-%d %H:%M:%S")
        conditions = {}
        for condition in checked_conditions:
            if condition == 'different_md5':  
                conditions[condition] = local_conds['md5'] != conds['md5']
            elif condition == 'higher_remote_time':
                conditions[condition] = remote_file_time > local_file_time
            else:
                conditions[condition] = conds['size'] > local_conds['size']

        logging.debug("Evaluated conditions for file {0}: {1}".format(filename, conditions))

        if not all(conditions.values()):
            # don't download remote file
            logging.info("File {0} does not meet conditions to be updated".format(filename))
            final_dict['invalid'].append(filename)
            del zip_file[filename[1:]]

    for name,content in zip_file.items():
        try:
            file_path = common.ossec_path + '/' + name
            remote_umask = CLUSTER_ITEMS[0]['umask'] if name in \
                           CLUSTER_ITEMS[0]['file_name'] else CLUSTER_ITEMS[1]['umask']
            remote_write_mode = CLUSTER_ITEMS[0]['write_mode'] if name in \
                           CLUSTER_ITEMS[0]['file_name'] else CLUSTER_ITEMS[1]['write_mode']
            _update_file(file_path, new_content=content['data'],
                            umask_int=remote_umask,
                            mtime=content['time'],
                            w_mode=remote_write_mode)

        except Exception as e:
            final_dict['error'].append({'item': name, 'reason': str(e)})
            continue

        final_dict['updated'].append(name)

    return final_dict


def divide_list(l, size=1000):
    return map(lambda x: filter(lambda y: y is not None, x), map(None, *([iter(l)] * size)))

def sync(debug, start_node=None, output_file=False, force=None):
    """
    Sync this node with others
    :return: Files synced.
    """
    def push_updates_single_node(all_files, node_dest, config_cluster, result_queue):
        # filter to send only pending files
        pending_files = filter(lambda x: x[1] != 'synchronized', all_files.items())
        if len(pending_files) > 0:
            logging.info("Sending {0} {1} files".format(node_dest, len(pending_files)))
            zip_file = compress_files(list_path=set(map(itemgetter(0), pending_files)))
            
            error, response = send_request(host=node_dest, port=config_cluster['port'],
                                           data="zip {0}".format(str(len(zip_file)).
                                            zfill(common.cluster_sync_msg_size - len("zip "))), 
                                           file=zip_file)
            
            try:
                res = literal_eval(response)
            except Exception as e:
                res = response

        else:
            logging.info("No pending files to send to {0} ".format(node_dest))
            res = {'data':{'updated':[], 'error':[], 'invalid':[]}}
            error = 0


        if error:
            logging.debug(res)
            result_queue.put({'node': node_dest, 'reason': "{0} - {1}".format(error, response),
                              'error': 1, 'files':{'updated':[], 'invalid':[], 
                                            'error':list(map(itemgetter(0), pending_files))}})
        else:
            logging.debug({'updated': len(res['data']['updated']), 
                          'error': res['data']['error'],
                          'invalid': res['data']['invalid']})
            result_queue.put({'node': node_dest, 'files': res['data'], 'error': 0, 'reason': ""})

    config_cluster = read_config()
    if not config_cluster:
        raise WazuhException(3000, "No config found")


    before = time()
    # Get own items status
    own_items = dict(filter(lambda x: not x[1]['is_synced'], get_files().items()))
    own_items_names = own_items.keys()
    all_nodes = get_nodes()['items']

    # Get connected nodes in the cluster
    cluster = [n['url'] for n in filter(lambda x: x['status'] == 'connected', 
                all_nodes)]
    # search the index of the localhost in the cluster
    try:
        localhost_index = cluster.index('localhost')
    except ValueError as e:
        logging.error("Cluster nodes are not correctly configured at ossec.conf.")
        exit(1)

    logging.info("Starting to sync {0}'s files".format(cluster[localhost_index]))

    cluster_socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    cluster_socket.connect("{0}/queue/ossec/cluster_db".format(common.ossec_path))

    logging.debug("Connected to cluster database socket")

    # for each connected manager, check its files. If the manager is not on database add it
    # with all files marked as pending
    all_nodes_files = {}
    remote_nodes = list(compress(cluster, map(lambda x: x != localhost_index, range(len(cluster)))))
    logging.debug("Nodes to sync: {0}".format(str(remote_nodes)))
    for node in remote_nodes:
        # check files in database
        count_query = "count {0}".format(node)
        cluster_socket.send(count_query)
        n_files = int(filter(lambda x: x != '\x00', cluster_socket.recv(10000)))
        if n_files == 0:
            logging.info("New manager found: {0}".format(node))
            logging.debug("Adding {0}'s files to database".format(node))

            # if the manager is not in the database, add it with all files
            for files in divide_list(own_items_names):

                insert_sql = "insert"
                for file in files:
                    insert_sql += " {0} {1}".format(node, file)

                cluster_socket.send(insert_sql)
                data = cluster_socket.recv(10000)

            all_files = {file:'pending' for file in own_items_names}

        else:
            logging.debug("Retrieving {0}'s files from database".format(node))
            # check node is in database
            # limit = 100 
            query = "select {0} 100 ".format(node)
            file_status = ""
            for offset in range(0,n_files,100):
                query += str(offset)
                cluster_socket.send(query)
                file_status += filter(lambda x: x != '\x00', cluster_socket.recv(10000))
                
            # retrieve all files for a node in database with its status
            all_files = {f[0]:f[1] for f in map(lambda x: x.split('*'), filter(lambda x: x != '', file_status.split(' ')))}
            # if there are missing files that are not being controled in database
            # add them as pending
            for missing in divide_list(set(own_items_names) - set(all_files.keys())):
                insert_sql = "insert"
                for m in missing:
                    all_files[m] = 'pending'
                    insert_sql += " {0} {1}".format(node,m)
            
                cluster_socket.send(insert_sql)
                data = cluster_socket.recv(10000)

        all_nodes_files[node] = all_files

    after = time()
    logging.debug("Time retrieving info from DB: {0}".format(after-before))

    before = time()
    result_queue = queue()
    threads = []
    thread_results = {}
    for node in remote_nodes:
        t = threading.Thread(target=push_updates_single_node, args=(all_nodes_files[node],node,
                                                                    config_cluster,
                                                                    result_queue))
        threads.append(t)
        t.start()
        result = result_queue.get()
        thread_results[result['node']] = {'files': result['files'], 'error': result['error'],
                                          'reason': result['reason']}

    for t in threads:
        t.join()
    after = time()
    
    logging.debug("Time sending info: {0}".format(after-before))

    before = time()
    for node,data in thread_results.items():
        logging.info("Updating {0}'s file status in DB".format(node))
        for updated in divide_list(data['files']['updated']):
            update_sql = "update2"
            for u in updated:
                update_sql += " synchronized {0} /{1}".format(node, u)
        
            cluster_socket.send(update_sql)
            received = cluster_socket.recv(10000)

        for failed in divide_list(data['files']['error']):
            update_sql = "update2"
            for f in failed:
                if isinstance(f, dict):
                    update_sql += " failed {0} /{1}".format(node, f['item'])
                else:
                    update_sql += " failed {0} {1}".format(node, f)

            cluster_socket.send(update_sql)
            received = cluster_socket.recv(10000)

        for invalid in divide_list(data['files']['invalid']):
            update_sql = "update2"
            for i in invalid:
                update_sql += " invalid {0} {1}".format(node, i)

            cluster_socket.send(update_sql)
            received = cluster_socket.recv(10000)

    cluster_socket.close()
    after = time()

    logging.debug("Time updating DB: {0}".format(after-before))

    if debug:
        return thread_results
    else:
        return {node:{'updated': len(data['files']['updated']), 
                      'error': data['files']['error'],
                      'invalid': data['files']['invalid'],
                      'error': data['error'],
                      'reason': data['reason']} 
                      for node,data in thread_results.items()}

