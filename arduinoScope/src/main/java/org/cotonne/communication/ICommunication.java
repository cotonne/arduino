package org.cotonne.communication;

import java.util.Set;
import java.util.HashSet;

public abstract class ICommunication{

        private Set<ICommunicationListener> listeners = new HashSet<ICommunicationListener>();

        public void addListener(ICommunicationListener listener){
                if(listener != null){
                        listeners.add(listener);
                }
        }

        protected void refresh(int[] data){
            if(data.length > 0){
                System.out.println(data.length);
                for(ICommunicationListener l : listeners){
                        l.update(data);
                }        
            } 
        }

}
