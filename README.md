# Mini-Bittorrent
### Requirement-
`sudo apt install openssl libssl-dev`

### Summary
[![Torrent-diagram.jpg](https://i.postimg.cc/T1nYXCCh/Torrent-diagram.jpg)](https://postimg.cc/MfK87bk8)

### Running the Project
* Run the following command to activate tracker server
 ```
 g++ -o tracker tracker.cpp -lpthread
 ```
 ``` 
 ./tracker <my_tracker_ip>:<my_tracker_port> <other_tracker_ip>:<other_tracker_port>  <seederlist_file>
 ```
* Run the following command to activate client server in client directory.
 ```
 g++ -o client client.cpp get_file.cpp remove_file.cpp seeder.cpp share_file.cpp socket.cpp torrent_creater.cpp -lpthread -lssl -lcrypto
 ```
 ```
 ./client <CLIENT_IP>:<UPLOAD_PORT> <TRACKER_IP_1>:<TRACKER_PORT_1> <TRACKER_IP_2>:<TRACKER_PORT_2>
 ```
  
#### Functions on Client side
###### Share
* It share the file over the network and can be accessible through mtorrent file.
```
share <local_file_path> <filename>.<file_extension>.mtorrent
```
###### Download
* It downloads the file mentioned in mtorrent from available seeder.
```
 get <path_to_.mtorrent_file> <destination_path>
```
###### Downloads Status
* Check which files are Downloaded.
```
show_downloads
```
###### Remove user from Seederlist
```
remove <filename.mtorrent>
```
###### Closing the client
```
close
```
