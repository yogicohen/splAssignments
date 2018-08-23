package bgu.spl181.net.impl.BBreactor;

import bgu.spl181.net.data.SharedProtocolData;
import bgu.spl181.net.impl.LineMessageEncoderDecoder;
import bgu.spl181.net.impl.MRSProtocol;
import bgu.spl181.net.srv.bidi.*;

public class ReactorMain {
	public static void main(String[] args) {
		
		SharedProtocolData data = new SharedProtocolData();
		data.LoadDataBase();
		
		Server.reactor(8,
				Integer.valueOf(args[0]),
				()-> new MRSProtocol(data),
				LineMessageEncoderDecoder::new).serve();
	}
}
