package org.cotonne.swing;

import java.awt.Font;

import javax.swing.JPanel; 
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.annotations.XYTextAnnotation;
import org.jfree.chart.axis.AxisLocation;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.plot.CombinedDomainXYPlot;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.StandardXYItemRenderer;
import org.jfree.chart.renderer.xy.XYItemRenderer;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

public class JChartPanel extends JPanel {

    private ChartPanel panel;
    
    public JChartPanel(){
        final JFreeChart chart = createCombinedChart();
        panel = new ChartPanel(chart, true, true, true, false, true);
        add(panel); 
    }

    private JFreeChart createCombinedChart() {

        // create subplot 1...
        final XYDataset data1 = createDataset1();
        final XYItemRenderer renderer1 = new StandardXYItemRenderer();
        final NumberAxis rangeAxis = new NumberAxis("Voltage");
        final NumberAxis domainAxis = new NumberAxis("Period");
        rangeAxis.setRange(0,256.0);
        domainAxis.setRange(0,20.0);
        final XYPlot plot = new XYPlot(data1, domainAxis, rangeAxis, renderer1);
        plot.setDomainZeroBaselineVisible(true);
        plot.setRangeZeroBaselineVisible(true);


        return new JFreeChart("", JFreeChart.DEFAULT_TITLE_FONT, plot, true);
        

    }
    
    private XYDataset createDataset1() {
        final XYSeries series1 = new XYSeries("Series 1");

        final XYSeriesCollection collection = new XYSeriesCollection();
        collection.addSeries(series1);
        return collection;

    }
    
    public void update(int[] data){
        final XYSeries series1 = new XYSeries("Series 1");
        for( int i  = 0; i < data.length; i++){
             series1.add(i, data[i]);
        }
       
        final XYSeriesCollection collection = new XYSeriesCollection();
        collection.addSeries(series1);
       
        JFreeChart c = panel.getChart();
        XYPlot plot = (XYPlot)c.getPlot();
        
        plot.getDomainAxis().setRange(0, data.length);
        
        plot.setDataset(0, collection);
        
        panel.repaint();
    }
}
