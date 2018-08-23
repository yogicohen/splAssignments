package bgu.spl181.net.srv.bidi;

import bgu.spl181.net.api.bidi.*;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.function.Supplier;

public abstract class BidiBaseServer<T> implements Server<T> {

    private final int port;
    private final Supplier<BidiMessagingProtocol<T>> protocolFactory;
    private final Supplier<BidiMessageEncoderDecoder<T>> encdecFactory;
    private ServerSocket sock;
    private ConnectionsImpl<T> serverConnections;
    private Integer counter = new Integer(0);

    public BidiBaseServer(
            int port,
            Supplier<BidiMessagingProtocol<T>> protocolFactory,
            Supplier<BidiMessageEncoderDecoder<T>> encdecFactory) {

        this.port = port;
        this.protocolFactory = protocolFactory;
        this.encdecFactory = encdecFactory;
		this.sock = null;
		this.serverConnections = new ConnectionsImpl<T>();
    }

    @Override
    public void serve() {

        try (ServerSocket serverSock = new ServerSocket(port)) {
			System.out.println("Server started");

            this.sock = serverSock; //just to be able to close

            while (!Thread.currentThread().isInterrupted()) {

                Socket clientSock = serverSock.accept();
                
                BidiMessagingProtocol<T> toInsert = protocolFactory.get();
                BidiBlockingConnectionHandler<T> handler = new BidiBlockingConnectionHandler<T>(
                        clientSock,
                        encdecFactory.get(),
                        toInsert);
                
                this.serverConnections.add(handler,counter);
                toInsert.start(counter, serverConnections);
                this.counter++;
                
                
                execute(handler);
            }
        } catch (IOException ex) {
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
		if (sock != null)
			sock.close();
    }

    protected abstract void execute(BidiBlockingConnectionHandler<T>  handler);

}
