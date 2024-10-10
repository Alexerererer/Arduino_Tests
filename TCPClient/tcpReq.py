import socket
import json

def send_json_over_tcp(server_ip, server_port, json_data):
    try:
        # Create a socket object
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        # Connect to the server
        client_socket.connect((server_ip, server_port))
        
        # Serialize the JSON object
        serialized_json = json.dumps(json_data)
        
        # Send the serialized JSON
        client_socket.sendall(serialized_json.encode())
        
        # Receive response from the server
        response = client_socket.recv(1024)
        print("Response from server:", response.decode())
        
        # Close the connection
        client_socket.close()
    
    except socket.error as e:
        print("Socket error:", e)

# Replace with the IP address and port of the server
server_ip = '192.168.1.15'  # Example: Replace with the server's IP
server_port = 80       # Example: Replace with the server's port

# Create a sample JSON object
data = {
    "a": "John hammer ",
    "b": "Me aaaaaaaaaaa "
}

# Sending the JSON object via TCP to the server
send_json_over_tcp(server_ip, server_port, data)