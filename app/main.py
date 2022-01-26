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
 
# global variables - data of current user
cl = Client()
username = ""
friends = []

class LoginScreen(Screen):

    # quick login function for devs - automatically logs into "test1" account
    # saves some time when debugging ;)
    def testing(self):
        self.ids["user"].text = "test1"
        self.ids["passwd"].text = "test1"
        self.login()

    # main login function
    def login(self):
        global cl
        global username

        # data dormat supported by the server
        data = {"username":self.ids["user"].text, "password":self.ids["passwd"].text}
        # console output for debugging
        print(f"login {data}")
        # API endpoint specification
        cl.post("login", json.dumps(data))
        # collect server response
        buff = cl.server.recv(4096)
        print(f"{buff}")

        # change screen if logged in
        if buff == b'200 : Logged in.':
            username = self.ids["user"].text
            print(f"Logged in as {username}!")
            self.manager.current = 'profile'   
        else:
            # update user with operation status
            self.ids["log_info"].text = "Login or password incorrect!"


class RegisterScreen(Screen):

    def register(self):
        global cl

        data = {"username":self.ids["ruser"].text, "password":self.ids["rpasswd"].text}
        print(f"signup {data}")

        # server response handling
        cl.post("signup", json.dumps(data))
        buff = cl.server.recv(4096)
        print(buff)

        if buff == b'201 : User created.':
            self.ids["reg_info"].text = "Account created!"
            self.ids["reg_info"].color = (0,1,0,0.75)
        elif buff == b'403 : User already exists.':
            self.ids["reg_info"].text = "User already exists!"
            self.ids["reg_info"].color = (1,0,0)

# supporting class for dropdown menu template
class IconListItem(OneLineIconListItem):
    icon = StringProperty()


class ProfileScreen(Screen):

    # this function activates each time when switching to shis screen
    def on_enter(self):
        #update list of friends of logged in user
        self.getFriends()
        global friends

        menu_items = [
            {
                "viewclass": "IconListItem",
                "icon": "git",
                "text": friends[i],
                "height": 56,
                "on_release": lambda x=friends[i]: self.set_item(x),
            } for i in range(len(friends))
        ]
        self.menu = MDDropdownMenu(
            caller=self.ids["drop_item"],
            items=menu_items,
            position="auto",
            width_mult=4,
        )
        self.menu.bind()

    def set_item(self, text_item):
        self.ids['drop_item'].set_item(text_item)
        self.ids['drop_item'].text = text_item
        self.menu.dismiss()

    def getFriends(self):
        global friends
        global cl
        global username

        data = {"username": username}
        print(f"get_friends {data}")
        cl.get("friends", json.dumps(data))
        
        
        buff = cl.server.recv(4096).decode('UTF-8')
        # server avoids sending empty messages
        # when a message has only 1 character, that means it's "empty"
        if len(buff) > 1:
            friends = buff[1:].split(",")
        print(friends)
    
    # get history of messages received from given user
    def getMessage(self):
        history = ""
        global cl
        global username

        data = {"sender": self.ids["drop_item"].text, "receiver": username}
        cl.get("messages", json.dumps(data))
        
        buff = ""
        while True:
            buff = cl.server.recv(4096).decode('UTF-8').rstrip('\x00')
            if self.ids["drop_item"].text == '<friend>':
                self.ids["fr_chat_info"].text = "First choose a friend!"
                break
            if buff == '200 : Messages received.':
                self.ids["fr_chat_info"].text = ""
                break
            elif buff == '404 : Users do not exist.':
                self.ids["fr_chat_info"].text = "User doesn't exist!"
                self.ids["fr_chat_info"].color = (1,0,0)
                break
            else:
                self.ids["fr_chat_info"].text = ""
            history += buff
            history += '\n'

        print(history)
        self.ids["fr_chat_field"].text = history

    def sendMessage(self):
        global cl
        global username

        data = {"sender": username, "receiver": self.ids["send_to"].text, "content": self.ids["chat_msg"].text}
        print(f"send {data}")
        cl.post("send", json.dumps(data))

        buff = cl.server.recv(4096)
        print(buff)

        # automatically update inbox after sending a message
        self.getMessage()
    

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
            self.ids["add_info"].text = "User doesn't exist!"
            self.ids["add_info"].color = (1,0,0)

# chat with user, whose username is typed in 
# (he/she doesn't have to be in friends list) 
class ChatScreen(Screen):

    def getMessage(self):
        history = ""
        global cl
        global username

        data = {"sender": self.ids["send_to"].text, "receiver": username}
        cl.get("messages", json.dumps(data))

        buff = ""
        while True:
            buff = cl.server.recv(4096).decode('UTF-8').rstrip('\x00')
            if buff == '200 : Messages received.':
                self.ids["chat_info"].text = ""
                break
            elif buff == '404 : Users do not exist.':
                self.ids["chat_info"].text = "User doesn't exist!"
                self.ids["chat_info"].color = (1,0,0)
                break
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

#main class, responsible for building the app
class DemoApp(MDApp):
    Window.size = (400, 600)
    def build(self):
        # because it ALWAYS look cooler
        self.theme_cls.theme_style = "Dark" 
        # load main .kv file
        screen = Builder.load_file('main.kv')
        return screen

# run main function
DemoApp().run()