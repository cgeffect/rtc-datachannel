import socket
import threading

# 存储两个对等端的连接
peer_connections = []

def handle_connection(client_socket):
    global peer_connections
    if len(peer_connections) < 2:
        peer_connections.append(client_socket)
        print(f"Peer connected. Total peers: {len(peer_connections)}")
        if len(peer_connections) == 2:
            # 开始交换 SDP 信息
            for i in range(2):
                sdp = peer_connections[i].recv(4096).decode()
                print(f"Received SDP from Peer {i + 1}: {sdp}")
                peer_connections[1 - i].sendall(sdp.encode())
    else:
        print("Max number of peers reached. Closing connection.")
        client_socket.close()

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('localhost', 8888))
    server_socket.listen(2)
    print("Signaling server started. Waiting for peers...")

    while True:
        client_socket, client_address = server_socket.accept()
        print(f"Accepted connection from {client_address}")
        client_thread = threading.Thread(target=handle_connection, args=(client_socket,))
        client_thread.start()

if __name__ == "__main__":
    start_server()