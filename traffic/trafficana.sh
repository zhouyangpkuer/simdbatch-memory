#!/bin/bash

nohup tshark -r equinix-chicago.dirA.20160121-130000.UTC.anon.pcap -T fields -e ip.src -e ip.dst -e tcp.srcport -e tcp.dstport -e ip.proto -E header=y -E separator=, -E occurrence=f > traffic_tcp.csv &
nohup tshark -r equinix-chicago.dirA.20160121-130000.UTC.anon.pcap -T fields -e ip.src -e ip.dst -e udp.srcport -e udp.dstport -e ip.proto -E header=y -E separator=, -E occurrence=f > traffic_udp.csv &
