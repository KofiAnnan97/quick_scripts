import sys, os, subprocess, re, tkinter
from tkinter import *
from tkinter import ttk
import platform

start_path = os.path.abspath(os.path.dirname(sys.argv[0]))
config_path = os.path.join(start_path, 'ping_list.txt') 

class Window(tkinter.Frame):
	def __init__(self, master=None):
		Frame.__init__(self, master)
		self.master = master

root = tkinter.Tk()

def check_config():
	if not os.path.isfile(config_path):
		root.title('Configuration Error')
		root.geometry('300x100')
		err = StringVar()
		err_label = Label(root, textvariable = err, pady=90)
		err.set('Configuration file is missing or has been moved.')
		err_label.pack()
		root.mainloop()
		sys.exit()
		
def ping_list_gen(ipfile):
	comment = '#'
	list = []
	for address in ipfile:
		if not re.match(comment,address) and address not in ['\n', '\r\n']:
			list.append(address.strip())
		else: 
			continue
	return list

def ping_func(ip, retries):
	try:
		parameter = "-n" if platform.system().lower() == "windows" else "-c"
		result = subprocess.check_output(["ping", parameter, str(retries), ip])
		return True
	except subprocess.CalledProcessError: #Ping Failed
		return False

def init(frame1, frame2):
		
	config_file = open(config_path,'r')
	pingArr = ping_list_gen(config_file)
	config_file.close()
	
	ping_font = ('times', 20)
		
	scrollbar = Scrollbar(frame1)
	scrollbar.pack(side=RIGHT, fill=Y)
	scroll_list = Listbox(frame1, yscrollcommand=scrollbar.set, font=ping_font)
	refresh(pingArr, scroll_list, config_path)
	scroll_list.pack(side=TOP, fill=BOTH, expand=True)
	scrollbar.config(command=scroll_list.yview)
	
	refresh_btn = Button(frame2, text='Refresh')
	refresh_btn.pack(side=LEFT, anchor=SW)
	refresh_btn.config(command=lambda: refresh(pingArr,scroll_list, config_path))
	
	execute_btn = Button(frame2, text='Execute')
	execute_btn.pack(side=RIGHT, anchor=SE)
	execute_btn.config(command=lambda: execute(pingArr,scroll_list))
	
	frame1.pack(side=TOP, fill=BOTH, expand=True)
	frame2.pack(side=BOTTOM, fill=X, padx = 70, pady = 10, expand=False)	
	
	root.after(300, execute, pingArr,scroll_list)

def refresh(lst, scr_lst, file_path):
	#print('Refresh Start')
	file = open(config_path, 'r')
	file_list = ping_list_gen(file)
	file.close()
	
	scr_lst.delete(0,END)
	for line in file_list:
		scr_lst.insert(END, str(line))
		scr_lst.itemconfig(END, bg='white')
	lst[:] = file_list
	root.update()
	#print('Refresh Done')

def execute(lst, scr_lst):
	#print('Execute Start')
	index = 0
	for ip_address in lst:
		if ping_func(ip_address, 4) == True:
			bgColor = 'green'
		else:
			bgColor ='red'
		#print(str(index) + " " + ip_address)
		scr_lst.itemconfig(index, bg=bgColor)
		root.update()
		index+=1
	#print('Execute Done')

def main():
	check_config()
	root.title('Ping Checker')
	root.geometry('300x370')#length x width
	root.minsize(width=400, height=370)
	root.maxsize(width=400, height=2000)

	ping_screen = tkinter.Frame(root)
	btn_screen = tkinter.Frame(root)

	init(ping_screen, btn_screen)
	
	root.mainloop()

if __name__== '__main__':
	main()