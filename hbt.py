
import socket
import struct
import select
import sys

p32 = lambda x: struct.pack("<I", x)
u32 = lambda bs: struct.unpack("<I", bs)[0]

class PIPE:
    def __init__(self, addr, port, debug = False):
	if debug:
             self.sok = SOK(addr, port, timeout = 1000)
        else:
             self.sok = SOK(addr, port)

        self.hbt = HBT(self.sok.sock)
	return

    def sendline(self, text):
        self.sok.sendline(text)
        return

    def recv(self, n):
        return self.sok.recv(n)

    def recvuntil(self, sk):
        return self.sok.recvuntil(sk)

    def recvline(self):
        return self.sok.recvline()

    def interactive(self):
        self.hbt.run()
        return

class SOK:
    def __init__(self, addr, port, timeout = 2):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((addr, port))
        self.sock.settimeout(timeout)
        self.buf = ''		#character buffer for stuff not yet accepted :)
        return

    # returns the characters in the bin
    def flush(self):
        r = self.buf
        self.buf = ''
        return r

    def push(self, s):
        self.buf += s
        return

    def sendline(self, text):
        self.sock.send(text + '\n')
        return

    def recv(self, n):
        return self.sock.recv(n)

    def recvuntil(self, sk):
	# first check bin
        if sk in self.buf:
            done = True
            r = self.flush()
	else:
            done = False
            r = ''

        while not done:
            rs, _, _ = select.select([self.sock], [], [])
            if self.sock in rs:
                data = self.sock.recv(4096)
                if not data or sk in data:
                    done = True
                    
		r += data

        cutoff = r.index(sk)
	self.push(r[cutoff + len(sk): ])
        return r[: cutoff + len(sk)]

    def recvline(self):
        return self.recvuntil('\n')

class HBT:
    def __init__(self, s, prompt = "shell> "):
        self.sock = s
        self.prompt = prompt
        return

    def output(self, s):
        sys.stdout.write(s) 
        sys.stdout.flush()
        return

    def run(self):
	print("<<<Starting interactive mode...")
        shellprompt = lambda: self.output(self.prompt)
        clearprompt = lambda: self.output("\b \b" * len(self.prompt))

        socklist = [sys.stdin, self.sock]
        done = False
        shellprompt()
        while not done:
            rs, ws, es = select.select(socklist, [], [])
            for rsa in rs:
                if rsa == self.sock:
                    data = self.sock.recv(4096)
                    if not data:
                        done = True
                        break

                    else:
			# somewhat inelegant, but it works
		        clearprompt()
                        print(data)
                        shellprompt()

                elif rsa == sys.stdin:
                    self.sock.send(raw_input() + '\n')
                    shellprompt()

	        else:
                    raise Exception("Unknown socket %s" % (str(rsa)))

        return
