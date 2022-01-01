import socket


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
    

    def get(self, path = None):
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
        
        self.server.send(bytes(f"{path} GET", "utf-8"))

        response = ""

        while True:
            chunk = self.server.recv(1024)
            if (len(chunk) <= 0):
                break
            response += chunk.decode("utf-8")

        return response
    

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
    

    def put(self, path = None, data = None):
        '''
        Standard PUT method updating data on the server.

        Parameters
        ----------
        path: str
            Endpoint of the server to update data on
        
        data: str
            Updated data
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
        
        self.server.send(bytes(f"{path} PUT {data}", "utf-8"))
