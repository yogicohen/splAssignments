package bgu.spl181.net.srv.bidi;

import java.util.HashMap;

import bgu.spl181.net.api.bidi.Connections;

public class ConnectionsImpl<T> implements Connections<T> {

	private HashMap<Integer, ConnectionHandler<T>> connections = new HashMap<>();
	
	public void add(ConnectionHandler<T> handler, int counter) {
		connections.put(counter, handler);
		
	}
	
	@Override
	public boolean send(int connectionId, T msg) {
		// TODO Auto-generated method stub
		if(!connections.containsKey(connectionId))
			return false;
		else connections.get(connectionId).send(msg);
		return true;
	}

	@Override
	public void broadcast(T msg) {
		// TODO Auto-generated method stub
		for(ConnectionHandler<T> current :connections.values()) {
			current.send(msg);
		}
	}

	@Override
	public void disconnect(int connectionId) {
		// TODO Auto-generated method stub
		connections.remove(connectionId);
	}
	

}
