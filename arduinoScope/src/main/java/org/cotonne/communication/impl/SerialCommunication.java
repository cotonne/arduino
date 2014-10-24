package org.cotonne.communication.impl;

import org.cotonne.communication.ICommunication;
import gnu.io.*;
import java.util.*;
import java.io.*;


/**
 * class to read serial port, mainly based on http://playground.arduino.cc/Interfacing/Java <br/>
 * Default serial speed : 115200 bps
 */
public class SerialCommunication extends ICommunication implements SerialPortEventListener  {
        private SerialPort port = null;
        private InputStream is = null;  // for demo purposes only. A stream would be more typical.
        private StringBuffer buffer = new StringBuffer();

        public SerialCommunication(String portName){
                System.setProperty("gnu.io.rxtx.SerialPorts", portName);
                try {
                        Enumeration portList = CommPortIdentifier.getPortIdentifiers();
                        while (portList.hasMoreElements()) {
                            CommPortIdentifier portId = (CommPortIdentifier) portList.nextElement();
                            System.out.println("Testing " + portId.getName());
                            if ((CommPortIdentifier.PORT_SERIAL == portId.getPortType()) && (portId.getName().equals(portName))) {
                                port = (SerialPort) portId.open(
                                    portName, // Name of the application asking for the port 
                                    10000   // Wait max. 10 sec. to acquire port
                                );
                                
                                port.setSerialPortParams(
                                        115200,
                                        SerialPort.DATABITS_8,
                                        SerialPort.STOPBITS_1,
                                        SerialPort.PARITY_NONE);
                            }
                        }
                } catch(PortInUseException e) {
                    System.err.println("Port already in use: " + e);
                    System.exit(1);
                } catch (UnsupportedCommOperationException e){
                    e.printStackTrace();
                    System.exit(1);
                }
                
                if(port == null){
                    System.err.println("Port not found : " + portName);
                    System.exit(1);
                }
                
                try {
                        is = port.getInputStream();
                } catch (IOException e) {
                    e.printStackTrace();
                        System.exit(1);
                }
    
                try{
                    port.addEventListener(this);
                } catch( TooManyListenersException e ){
                    e.printStackTrace();
                }
                port.notifyOnDataAvailable(true);
        }
        
    synchronized public void serialEvent(SerialPortEvent serialEvent) {
    if (serialEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
      try {
        while (is.available() > 0) {
            int c = is.read();
            buffer.append((char)c);
            if( c == 13 ){
                String response = buffer.toString().trim();
                int size = response.length()/2;
                char[] data = new char[2];
                int[] array = new int[size];
                for( int i  = 0; i < size; i++){
                    response.getChars(2*i, 2*(i+1), data, 0) ;
                    array[i] = Integer.parseInt(String.valueOf(data), 16);
                }
                refresh(array);
                buffer = new StringBuffer();
            }        
          }
      } catch (IOException e) {
        e.printStackTrace();
      } catch (Exception e) {
        e.printStackTrace();
      }
    }
  }

        
        public void close(){
            try{
                if (is != null){
                        is.close();
                }
            } catch (IOException e) {
                    e.printStackTrace();
                }
        }



}
