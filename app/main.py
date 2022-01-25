from http import client
from kivymd.app import MDApp
from kivy.lang.builder import Builder
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.uix.label import Label
from kivymd.uix.menu import MDDropdownMenu
from kivy.core.window import Window
from kivy.properties import StringProperty
from kivymd.uix.list import OneLineIconListItem
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
 
cl = Client()
username = ""
class LoginScreen(Screen):
    def login(self):
        global cl
        global username
        data = {"username":self.ids["user"].text, "password":self.ids["passwd"].text}
        print(f"login {data}")
        cl.post("login", json.dumps(data))
        buff = cl.server.recv(4096)
        print(f"{buff}")
        if buff == b'200 : Logged in.':
            username = self.ids["user"].text
            print(f"Logged in as {username}!")
            self.manager.current = 'profile'
            
        else:
            self.ids["log_info"].text = "Login or password incorrect!"
        #cl.get("messages", json.dumps(data))
    def admin_test(self, cl):
        print("Testing mode...")
        data = {"sender":"admin", "receiver":"test"}
        cl.get("messages", json.dumps(data))

class RegisterScreen(Screen):
    def register(self):
        global cl
        data = {"username":self.ids["ruser"].text, "password":self.ids["rpasswd"].text}
        print(f"signup {data}")
        cl.post("signup", json.dumps(data))
        buff = cl.server.recv(4096)
        print(buff)

class IconListItem(OneLineIconListItem):
    icon = StringProperty()

class ProfileScreen(Screen):
    
    def on_enter(self):
        menu_items = [
            {
                "viewclass": "IconListItem",
                "icon": "git",
                "text": f"Item {i}",
                "height": 56,
                "on_release": lambda x=f"Item {i}": self.set_item(x),
            } for i in range(5)
        ]
        self.menu = MDDropdownMenu(
            caller=self.ids["drop_item"],
            items=menu_items,
            position="center",
            width_mult=4,
        )
        self.menu.bind()

    def set_item(self, text_item):
        self.screen.ids.drop_item.set_item(text_item)
        self.menu.dismiss()
    def getFriends(self):
        self.items = [f"Item {i}" for i in range(50)]
        global cl
        global username
        data = {"username": username}
        print(f"get_friends {data}")
        cl.get("friends", json.dumps(data))
        buff = cl.server.recv(4096).decode('UTF-8')
        self.items = buff.split(",")
        #self.ids["friend_list"].items = self.items
        print(self.items)
    
class AddFriendScreen(Screen):
    def addFriend(self):
        global cl
        global username
        data = {'username': username,'friend': self.ids["fname"].text}
        cl.post("friends", json.dumps(data))
        buff = cl.server.recv(4096)
        print(f"addFriend {buff}")
        if buff == b'201 : Friend added.':
            self.ids["add_info"].text = "A friend was added!"
            self.ids["add_info"].color = (0,1,0,0.75)
        elif buff == b'403 : User does not exist.':
            self.ids["add_info"].text = "Wrong username!"
            self.ids["add_info"].color = (1,0,0)

class ChatScreen(Screen):
    def getMessage(self):
        history = ""
        global cl
        global username
        data = {"sender": self.ids["send_to"].text, "receiver": username}
        cl.get("messages", json.dumps(data))
        buff = "x"
        while True:
            buff = cl.server.recv(4096).decode('UTF-8').rstrip('\x00')
            if buff == '200 : Messages received.':
                self.ids["chat_info"].text = ""
                break
            elif buff == b'404 : Users do not exist.':
                self.ids["chat_info"].text = "Wrong username!"
                self.ids["chat_info"].color = (1,0,0)
            else:
                self.ids["chat_info"].text = ""
            history += buff
            history += '\n'
            #print(buff)
        print(history)
        self.ids["chat_field"].text = history
    def sendMessage(self):
        global cl
        global username
        data = {"sender": username, "receiver": self.ids["send_to"].text, "content": self.ids["chat_msg"].text}
        print(f"send {data}")
        cl.post("send", json.dumps(data))
        buff = cl.server.recv(4096)
        print(buff)
        self.getMessage()

    


# Create the screen manager
sm = ScreenManager()
sm.add_widget(LoginScreen(name='login'))
sm.add_widget(RegisterScreen(name='register'))
sm.add_widget(ProfileScreen(name='profile'))
sm.add_widget(AddFriendScreen(name='add'))
sm.add_widget(ChatScreen(name='upload'))


class DemoApp(MDApp):
    Window.size = (300, 450)
    def build(self):
        screen = Builder.load_file('main.kv')
        return screen


DemoApp().run()