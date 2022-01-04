"""
Example script for sending data to server.
By no means is it complete client API and 
does not have all functionality (i.e. fetching all messages).
For testing purposes only.
"""

import socket
import json


class Client:
    """
    Client for bsd-sockets written in Python. Connects to server written in C.

    Parameters
    ----------
    port : int
    default : 1100
        Port of the server
    
    ip : str
    default: localhost
        IP of the server
    """


    def __init__(self, port = 1100, ip = socket.gethostbyname(socket.gethostname())):
        self.server_port = port
        self.server_ip = ip
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.connect((self.server_ip, self.server_port))
    

    def __del__(self):
        self.server.close()
    

    def get(self, path = None, data = None):
        '''
        Standard GET method returning data from server.

        Parameters
        ----------
        path: str
            Endpoint of the server to get data from
        
        Returns
        -------
        response: str
            Full response from the server
        '''
        if path == None:
            print ("No path specified for GET, returning...")
            return
        
        if not isinstance(data, str) and data is not None:
            print("Data in incorrect format, returning...")
            return
        
        self.server.send(bytes(f"{path} GET {data}", "utf-8"))
    

    def post(self, path = None, data = None):
        '''
        Standard POST method sending data to the server.

        Parameters
        ----------
        path: str
            Endpoint of the server to send data to
        
        data: str
            Data to send to the server
        '''
        if path == None:
            print("No path specified for POST, returning...")
            return
        
        if data == None:
            print("No data specified for POST, returning...")
            return
        
        if not isinstance(data, str):
            print("Data in incorrect format, returning...")
            return
        
        self.server.send(bytes(f"{path} POST {data}", "utf-8"))
    

if __name__ == "__main__":
    client = Client()
    # Login
    # data = {"username":"test", "password": "user"}

    # Signup
    # data = {"username: "test", "password": "user"}

    # Friends
    # data = {"username": "test"}

    # Messages
    # data = {"sender": "test", "receiver": "other"}

    # Send message
    # data = {"sender": "test", "receiver": "other", "content": "blabla"}

    # Refer to C server file for further guidelines
    data = {"sender":"admin", "receiver":"test"}
    # API endpoint specification
    client.get("messages", json.dumps(data))
