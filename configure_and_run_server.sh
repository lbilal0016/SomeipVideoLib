sudo ip route add 224.224.224.245/32 dev enp0s3
sudo iptables -A INPUT -p udp -d 224.224.224.245 --dport 30490 -j ACCEPT
_config_path="/home/ubuntu1/SomeipLibProject/config_server"
export VSOMEIP_CONFIGURATION="$_config_path"
echo "$VSOMEIP_CONFIGURATION"
./build/service "/home/ubuntu1/SomeipLibProject/config_server/VideoConfig.json" 
