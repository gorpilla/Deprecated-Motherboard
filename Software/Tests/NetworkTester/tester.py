from Tkinter import *
#from Tkinter import Ttk
import socket
    
def sendmessage(*args):
    global connection
    to_send = bytearray.fromhex(Data.get())
    connection.send(to_send)
    print "Sent data ", to_send

root = Tk()
root.title("NetworkTestUtility")

print "Waiting for Connection"
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((socket.gethostname(), 11000))
server.listen(5)
connection, addr = server.accept()
print "Connected"


Data = StringVar()

Label(root, text="Data: 0x").pack(side=TOP)
data_entry = Entry(root, width=24, textvariable = Data)
data_entry.pack(side=TOP)
sendbutton = Button(root, text="Send", command=sendmessage).pack(side=TOP)

data_entry.focus()
root.bind('<Return>', sendmessage)
try:
    root.mainloop()
except:
    print "Shit hit the fan"
    connection.close()
    server.close()
    quit()

