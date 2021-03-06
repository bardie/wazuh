#!/usr/bin/env python

# Created by Wazuh, Inc. <info@wazuh.com>.
# This program is a free software; you can redistribute it and/or modify it under the terms of GPLv2

import asyncore
import socket
import json
from distutils.util import strtobool
from sys import argv, exit, path
from os.path import dirname
from subprocess import check_call, CalledProcessError, check_output
from os import devnull, seteuid, setgid, getpid, kill
from multiprocessing import Process
from re import search
from time import sleep
from pwd import getpwnam
from signal import signal, SIGINT
import ctypes
import ctypes.util

import logging
logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s: %(message)s',
                    filename="/var/ossec/logs/cluster.log")

import argparse
parser =argparse.ArgumentParser()
parser.add_argument('-f', help="Run in foreground", action='store_true')
parser.add_argument('-d', help="Enable debug messages", action='store_true')
parser.add_argument('-V', help="Print version", action='store_true')

# Set framework path
path.append(dirname(argv[0]) + '/../framework')  # It is necessary to import Wazuh package

child_pid = 0

# Import framework
try:
    from wazuh import Wazuh
    from wazuh.common import *
    from wazuh.cluster import *
    from wazuh.exception import WazuhException
    from wazuh.InputValidator import InputValidator
    from wazuh.utils import send_request
    from wazuh.pyDaemonModule import pyDaemon, create_pid, delete_pid
    iv = InputValidator()
except Exception as e:
    print("Error importing 'Wazuh' package.\n\n{0}\n".format(e))
    exit()

class WazuhClusterHandler(asyncore.dispatcher_with_send):
    def __init__(self, sock, addr):
        asyncore.dispatcher_with_send.__init__(self, sock)
        self.addr = addr

    def handle_close(self):
        self.close()

    def handle_read(self):
        error = 0
        res = ""
        try:
            recv_command = self.recv(common.cluster_sync_msg_size).decode()

            if recv_command == '':
                self.handle_close()
                return

            command = recv_command.split(" ")

            logging.debug("Command received: {0}".format(command))

            if not iv.check_cluster_cmd(command):
                logging.error("received unvalid cluster command {0} from {1}".format(command[0], self.addr))
                error = 1
                res = "Received unvalid cluster command {0}".format(command[0])

            if error == 0:
                if command[0] == "node":
                    res = get_node()
                elif command[0] == "zip":
                    zip_bytes = self.recv(int(command[1]))
                    if not zip_bytes:
                        raise "Received empty zip file"
                        return
                    logging.debug("Zip file received from {0}".format(self.addr))
                    res = extract_zip(zip_bytes)

                logging.debug("Command {0} executed for {1}".format(command[0], self.addr))

            data = json.dumps({'error': error, 'data': res})

        except Exception as e:
            logging.error("Error handling client request: {0}".format(str(e)))
            data = json.dumps({'error': 1, 'data': str(e)})
        
        self.send(data + '\n')
        logging.debug("Data sent to {0}".format(self.addr))
        # self.handle_close()

class WazuhClusterServer(asyncore.dispatcher):

    def __init__(self, host, port):
        asyncore.dispatcher.__init__(self)
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.set_reuse_addr()
        try:
            self.bind((host, port))
        except socket.error as e:
            logging.error("Can't bind socket: {0}".format(str(e)))
            raise e
        self.listen(50)
        logging.info("Listening on port {0}.".format(port))

    def handle_accept(self):
        pair = self.accept()
        if pair is not None:
            sock, addr = pair
            logging.info("Accepted connection from host {0}".format(addr[0]))
            handler = WazuhClusterHandler(sock, addr[0])
        return

    def handle_error(self):
        nil, t, v, tbinfo = asyncore.compact_traceback()
        raise t(v)


def crontab_sync(interval):
    interval_number  = int(search('\d+', interval).group(0))
    interval_measure = interval[-1]
    while True:
        logging.debug("Crontab: starting to sync")
        sync(False)
        sleep(interval_number if interval_measure == 's' else interval_number*60)

def signal_handler(n_signal, frame):
    def strsignal(n_signal):
        libc = ctypes.CDLL(ctypes.util.find_library('c'))
        strsignal_proto = ctypes.CFUNCTYPE(ctypes.c_char_p, ctypes.c_int)
        strsignal_c = strsignal_proto(("strsignal", libc), ((1,),))

        return strsignal_c(n_signal)

    logging.info("Signal [{0}-{1}] received. Exit cleaning...".format(n_signal, 
                                                               strsignal(n_signal)))
    # received Cntrl+C
    if n_signal == SIGINT:
        # kill C daemon if it's running
        try:
            pid = int(check_output(["pidof","{0}/framework/wazuh-clusterd-internal".format(ossec_path)]))
            kill(pid, SIGINT)
        except CalledProcessError:
            pass

        if child_pid != 0:
            # remove pid files
            delete_pid("wazuh-clusterd", getpid())
    exit(1)

if __name__ == '__main__':
    # Drop privileges to ossec
    pwdnam_ossec = getpwnam('ossec')
    setgid(pwdnam_ossec.pw_gid)
    seteuid(pwdnam_ossec.pw_uid)

    args = parser.parse_args()
    if args.V:
        check_output(["{0}/bin/wazuh-clusterd-internal".format(ossec_path), '-V'])
        exit(0)

    # Capture Cntrl + C
    signal(SIGINT, signal_handler)

    if not args.f:
        res_code = pyDaemon()
    else:
        # define a Handler which writes INFO messages or higher to the sys.stderr
        console = logging.StreamHandler()
        console.setLevel(logging.DEBUG)
        # set a format which is simpler for console use
        formatter = logging.Formatter('%(asctime)s %(levelname)s: %(message)s')
        # tell the handler to use this format
        console.setFormatter(formatter)
        # add the handler to the root logger
        logging.getLogger('').addHandler(console)

    create_pid("wazuh-clusterd", getpid())

    if not args.d:
        logging.getLogger('').setLevel(logging.INFO)

    # execute C cluster daemon (database & inotify) if it's not running
    try:
        exit_code = check_call(["ps", "-C", "wazuh-clusterd-internal"], stdout=open(devnull, 'w'))
    except CalledProcessError:
        check_call(["{0}/bin/wazuh-clusterd-internal".format(ossec_path)])
    
    # Initialize framework
    myWazuh = Wazuh(get_init=True)
    
    cluster_config = read_config()
    # execute an independent process to "crontab" the sync interval
    p = Process(target=crontab_sync, args=(cluster_config['interval'],))
    if not args.f:
        p.daemon=True
    p.start()
    child_pid = p.pid

    server = WazuhClusterServer('' if not cluster_config['host'] else cluster_config['host'], 
                                int(cluster_config['port']))
    asyncore.loop()
