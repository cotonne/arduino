package org.cotonne.swing;

import javax.swing.JFrame;
import javax.swing.JLabel;
import java.awt.BorderLayout;
import org.jfree.ui.ApplicationFrame;
import org.cotonne.communication.ICommunicationListener;

public class JGraph extends ApplicationFrame implements ICommunicationListener {

    private JChartPanel chartPanel;

    public JGraph(){
        super("arduinoScope");
        chartPanel = new JChartPanel();
        
        
        getContentPane().add(new JLabel("Other operation"), BorderLayout.NORTH);
        getContentPane().add(chartPanel, BorderLayout.CENTER);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(100, 100);
        setContentPane(chartPanel);
    }
    
    public void update(int[] data){
        chartPanel.update(data);
    }
}
