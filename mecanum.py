# Import necessary modules
import network
import socket
import time
import random
from machine import Pin, PWM

# Create an LED object on pin 'LED'
led = Pin('LED', Pin.OUT)

# Create four motor driver objects
motor_a_forward = Pin(18, Pin.OUT)
motor_a_backward = Pin(19, Pin.OUT)
motor_b_forward = Pin(20, Pin.OUT)
motor_b_backward = Pin(21, Pin.OUT)
motor_c_forward = Pin(12, Pin.OUT)
motor_c_backward = Pin(13, Pin.OUT)
motor_d_forward = Pin(10, Pin.OUT)
motor_d_backward = Pin(11, Pin.OUT)


# Wi-Fi credentials
ssid = 'robosoccer'
password = 'iitmadras'

def move_forward():
    motor_a_forward.value(1)
    motor_b_forward.value(1)
    motor_a_backward.value(0)
    motor_b_backward.value(0)
    motor_c_forward.value(1)
    motor_d_forward.value(1)
    motor_c_backward.value(0)
    motor_d_backward.value(0)

def move_backward():
    motor_a_forward.value(0)
    motor_b_forward.value(0)
    motor_a_backward.value(1)
    motor_b_backward.value(1)
    motor_c_forward.value(0)
    motor_d_forward.value(0)
    motor_c_backward.value(1)
    motor_d_backward.value(1)

def move_stop():
    motor_a_forward.value(0)
    motor_b_forward.value(0)
    motor_a_backward.value(0)
    motor_b_backward.value(0)
    motor_c_forward.value(0)
    motor_d_forward.value(0)
    motor_c_backward.value(0)
    motor_d_backward.value(0)
    
def move_right():
    motor_a_forward.value(1)
    motor_b_forward.value(0)
    motor_a_backward.value(0)
    motor_b_backward.value(1)
    motor_c_forward.value(0)
    motor_d_forward.value(1)
    motor_c_backward.value(1)
    motor_d_backward.value(0)

def move_left():
    motor_a_forward.value(0)
    motor_b_forward.value(1)
    motor_a_backward.value(1)
    motor_b_backward.value(0)
    motor_c_forward.value(1)
    motor_d_forward.value(0)
    motor_c_backward.value(0)
    motor_d_backward.value(1)
    
def spin_right():
    motor_a_forward.value(1)
    motor_b_forward.value(0)
    motor_a_backward.value(0)
    motor_b_backward.value(1)
    motor_c_forward.value(1)
    motor_d_forward.value(0)
    motor_c_backward.value(0)
    motor_d_backward.value(1)
    time.sleep(0.1)
    move_stop()
    
def spin_left():
    motor_a_forward.value(0)
    motor_b_forward.value(1)
    motor_a_backward.value(1)
    motor_b_backward.value(0)
    motor_c_forward.value(0)
    motor_d_forward.value(1)
    motor_c_backward.value(1)
    motor_d_backward.value(0)
    time.sleep(0.1)
    move_stop()    

def diagonal_right_forward():
    motor_a_forward.value(1)
    motor_b_forward.value(0)
    motor_a_backward.value(0)
    motor_b_backward.value(0)
    motor_c_forward.value(0)
    motor_d_forward.value(1)
    motor_c_backward.value(0)
    motor_d_backward.value(0)

def diagonal_left_forward():
    motor_a_forward.value(0)
    motor_b_forward.value(1)
    motor_a_backward.value(0)
    motor_b_backward.value(0)
    motor_c_forward.value(1)
    motor_d_forward.value(0)
    motor_c_backward.value(0)
    motor_d_backward.value(0)
    
def diagonal_right_backward():
    motor_a_forward.value(0)
    motor_b_forward.value(0)
    motor_a_backward.value(0)
    motor_b_backward.value(1)
    motor_c_forward.value(0)
    motor_d_forward.value(0)
    motor_c_backward.value(1)
    motor_d_backward.value(0)

def diagonal_left_backward():
    motor_a_forward.value(0)
    motor_b_forward.value(0)
    motor_a_backward.value(1)
    motor_b_backward.value(0)
    motor_c_forward.value(0)
    motor_d_forward.value(0)
    motor_c_backward.value(0)
    motor_d_backward.value(1)
    
def zigzag():
    for _ in range(3):
        diagonal_left_forward()
        time.sleep(0.5)
        diagonal_right_forward()
        time.sleep(0.5)
        diagonal_left_backward()
        time.sleep(0.5)
        diagonal_right_backward()
        time.sleep(0.5)
    move_stop()

move_stop()

# HTML template for the webpage
def webpage(random_value, state):
    html = f"""
        <!DOCTYPE html>
        <html>
        <head>
            <title>Aditya Rao - Mecanum - Paradox'24 Workshop</title>
            <meta name="viewport" content="width=device-width, initial-scale=1">
        </head>
        <body>
            <h1 align="center">ROBO SOCCER WIFI CONTROL</h1>
            <h2 align="center">Control</h2>
            <table align="center">
            <tr>
                <td>
                    <form action="./lighton">
                        <input type="submit" value="Light on" style="width:120px; height:120px;"/>
                    </form>
                </td>
                <td>
                    <form action="./lightoff">
                        <input type="submit" value="Light off" style="width:120px; height:120px;"/>
                    </form>
                </td>
            </tr>
            </table>
            <br />
            <table align="center">
			<tr>
				<td>
					<form action="./forwardleftdiagonal">
                        <input type="submit" value="Forward Left Diagonal" style="width:120px; height:120px;"/>
                    </form>
				</td>
				<td>
                    <form action="./forward">
                        <input type="submit" value="Forward" style="width:120px; height:120px;"/>
                    </form>
                </td>
				<td>
					<form action="./forwardrightdiagonal">
                        <input type="submit" value="Forward Right Diagonal" style="width:120px; height:120px;"/>
                    </form>
				</td>
			</tr>
            <tr>                
                <td>
                    <form action="./left">
                        <input type="submit" value="Left" style="width:120px; height:120px;"/>
                    </form>
                </td>
				<td>
                    <form action="./stop">
                        <input type="submit" value="Stop" style="width:120px; height:120px;"/>
                    </form>
                </td>
                <td>
                    <form action="./right">
                        <input type="submit" value="Right" style="width:120px; height:120px;"/>
                    </form>
                </td>
            </tr>
			<tr>
				<td>
					<form action="./backwardleftdiagonal">
                        <input type="submit" value="Backward Left Diagonal" style="width:120px; height:120px;"/>
                    </form>
				</td>
				<td>
                    <form action="./backward">
                        <input type="submit" value="Backward" style="width:120px; height:120px;"/>
                    </form>
                </td>
				<td>
					<form action="./backwardrightdiagonal">
                        <input type="submit" value="Backward Right Diagonal" style="width:120px; height:120px;"/>
                    </form>
				</td>
			</tr>
            </table>
            <br />
			<table align="center">
            <tr>
                <td>
                    <form action="./leftrotate">
                        <input type="submit" value="Left Rotate" style="width:120px; height:120px;"/>
                    </form>
                </td>
                <td>
                    <form action="./rightrotate">
                        <input type="submit" value="Right Rotate" style="width:120px; height:120px;"/>
                    </form>
                </td>
            </tr>
            
            <br />
            
            <p align="center">Status: {state}</p>
			</table>
			<table align="center"><tr><td>
            <h2 align="center">Fetch New Value</h2>
			</td></tr><tr align="center"><td>
            <form action="./value">
                <input type="submit" value="Fetch value" style="width:120px; height:120px;"/>
            </form>
            <p align="center">Fetched value: {random_value}</p>
			</td></tr></table>
        </body>
        </html>
        """
    return str(html)

# Connect to WLAN
wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(ssid, password)

# Wait for Wi-Fi connection
connection_timeout = 10
while connection_timeout > 0:
    if wlan.status() >= 3:
        break
    connection_timeout -= 1
    print('Waiting for RoboSoccer Wi-Fi connection...')
    time.sleep(1)

# Check if connection is successful
if wlan.status() != 3:
    raise RuntimeError('Failed to establish a network connection with RoboSoccer WiFi')
else:
    print('Connection to RoboSoccer network successful!')
    network_info = wlan.ifconfig()
    print('IP address:', network_info[0])

# Set up socket and start listening
addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
s = socket.socket()
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind(addr)
s.listen()

print('Listening on', addr)

# Initialize variables
state = "OFF"
random_value = 0

# Main loop to listen for connections
while True:
    try:
        conn, addr = s.accept()
        print('Got a connection from', addr)
        
        # Receive and parse the request
        request = conn.recv(1024)
        request = str(request)
        print('Request content = %s' % request)

        try:
            request = request.split()[1]
            print('Request:', request)
        except IndexError:
            pass
        
        # Process the request and update variables
        if request == '/lighton?':
            print("LED on")
            led.value(1)
            state = "ON"
        elif request == '/lightoff?':
            led.value(0)
            state = 'OFF'
        elif request == '/forward?':
            move_forward()
            state = 'Forward'
        elif request == '/backward?':
            move_backward()
            state = 'Backward'
        elif request == '/stop?':
            move_stop()
            state = 'Stop'
        elif request == '/left?':
            move_left()
            state = 'Left'
        elif request == '/right?':
            move_right()
            state = 'Right'
        elif request == '/leftrotate?':
            spin_left()
            state = 'Spin left'
        elif request == '/rightrotate?':
            spin_right()
            state = 'Spin right'
        elif request == '/backwardrightdiagonal?':
             diagonal_right_backward()
             state = 'Backward right diagnal'
        elif request == '/backwardleftdiagonal?':
             diagonal_left_backward()
             state = 'Backward left diagnal'
        elif request == '/forwardrightdiagonal?':
             diagonal_right_forward()
             state = 'forward right diagnal'
        elif request == '/forwardleftdiagonal?':
             diagonal_left_forward()
             state = 'forward left diagnal'
        elif request == '/zigzag?':
            zigzag()
            state = 'Zigzag'
        elif request == '/value?':
            random_value = random.randint(10, 20)

        # Generate HTML response
        response = webpage(random_value, state)  

        # Send the HTTP response and close the connection
        conn.send('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')
        conn.send(response)
        conn.close()

    except OSError as e:
        conn.close()
        print('Connection closed')
