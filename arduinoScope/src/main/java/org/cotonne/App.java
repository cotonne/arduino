package org.cotonne;

import org.cotonne.swing.JGraph;
import org.cotonne.communication.impl.SerialCommunication;
/**
 * Hello world!
 *
 */
public class App {

    public static void main( String[] args ) {
        System.out.println( "arduinoScope" );
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                SerialCommunication serial = new SerialCommunication("/dev/ttyACM0");
                JGraph graph = new JGraph();
                
                serial.addListener(graph);
                graph.pack();
                graph.setVisible(true);
            }
        });
    }
}
