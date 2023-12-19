import socket
import sys #este pentru detectare eroare
host = ''
port = 5560

storedValue = "Functioneaza"



def setupServer():
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	print("SOCKET CREATED")
	try:
		s.bind((host,port))
	except socket.error as msg:
		print(msg)
	print("Socket bind complete.")
	return s

def setupConnection():
	s.listen(1) #Allows one connection at a time
	conn, address = s.accept()
	print("Connected to: "+address[0]+":"+ str(address[1]))
	return conn


def dataTransfer(conn,trimitere):
	#a big loop that sends/receives data until told no to
	while True:
		#receive the data
		data = conn.recv(1024) #receive the data cat de mult vrei
		data = data.decode('utf-8') #data este received ca bits si trebuie sa fie decodata, diferentiat bits to string data
		#split the data sch that you separate de command
		#from the rest of the dataq
		dataMessage = data.split(' ',1)
		command = dataMessage[0]
		if command == 'GET':
			reply = trimitere
		elif command == 'EXIT':
			print('Our client has left us:')
			break
		elif command == 'KILL':
			print('Our server is shutting down')
			s.close()
			break
		else:
			reply = "Unknown Command"
		##send the reply back to the client
		conn.sendall(str.encode(reply))
		print('Data has been sent!')
	conn.close()


s = setupServer()
while True:
	try:
		conn = setupConnection()
		dataTransfer(conn,"text")
	except:
		print("Unexpected error:", sys.exc_info()[0])
		break


