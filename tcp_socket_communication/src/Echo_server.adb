with Ada.Text_IO;
with Ada.IO_Exceptions;
with GNAT.Sockets;
with Ada.Streams;
use type Ada.Streams.Stream_Element_Count;
procedure Echo_server is
	--TCP socket stuff
	Receiver   : GNAT.Sockets.Socket_Type;
	Connection : GNAT.Sockets.Socket_Type;
	Client     : GNAT.Sockets.Sock_Addr_Type;
	Channel    : GNAT.Sockets.Stream_Access;
	Offset : Ada.Streams.Stream_Element_Count;
	--Data   : Ada.Streams.Stream_Element_Array (1 .. 1024);
  Data_Stream : Ada.Streams.Stream_Element_Array (1 .. Ada.Streams.Stream_Element_Offset(Data'Length));

			--Send the data onward.
	
	dataString : string(1 .. 50); --A string to store received data, before converting it to floats.
	ii : Integer := 1; --Loop var, string posision for recieved data.

	--The varibles that is used to send the data onward.
	X : FLoat := 0.0;
	Y : Float := 0.0;
	Z : Float := 0.0;
	Roll : Float := 0.0;
	Pitch : Float := 0.0;
	Yaw : Float := 0.0;
begin

	--Inisialize the socket.
	Ada.Text_IO.Put_Line("Starting server");
	Ada.Text_IO.Put_Line("Creating socket");
	GNAT.Sockets.Create_Socket (Socket => Receiver);
	GNAT.Sockets.Set_Socket_Option
	   (Socket => Receiver, Option => (Name    => GNAT.Sockets.Reuse_Address, Enabled => True));
	GNAT.Sockets.Bind_Socket
	   (Socket  => Receiver, Address => (Family => GNAT.Sockets.Family_Inet, 
	   Addr   => GNAT.Sockets.Inet_Addr ("127.0.0.1"), Port   => 12321));

	Ada.Text_IO.Put_Line("Listening for connection...");
	GNAT.Sockets.Listen_Socket (Socket => Receiver);

	loop

		--Wait for a client to connect.
		Ada.Text_IO.Put_Line("Waiting for connection...");
     		GNAT.Sockets.Accept_Socket
	             (Server  => Receiver,
	              Socket  => Connection, Address => Client);
		Ada.Text_IO.Put_Line("Client connected from " & GNAT.Sockets.Image (Client));
		Channel := GNAT.Sockets.Stream (Connection);
		begin
			
			--Reed the data in the socket.
			Ada.Text_IO.Put_Line("Sending JSON string.");
      dataString := "{""id"": 2, ""name"": ""abc""}";
      for I in Data_Stream'Range loop
           Data_Stream(I) := Ada.Streams.Stream_Element(Character'Pos(Data(Integer(I))));
           --Ada.Text_IO.Put(Character'Val(Data_Stream(I)));
      end loop;

			loop
        


				--Ada.Streams.Read(Channel.All, Data, Offset);
				--Ada.Text_IO.Put_Line("Printing received data: ");
		        	--Character'Output (Channel, Character'Input (Channel));
				--exit when Offset = 0;
				--ii := 1;

				--Store the data to dataString so that it can be converted to a float.
				--for I in 1 .. Offset loop
					--dataString(ii) := Character'Val(Data(I));
					--ii := ii + 1;
				--end loop;
			end loop;
			--Ada.Text_IO.Put_Line(dataString);
			--Convert and store the data from the string to correct varibles.
			--X := Float'Value(dataString(1 .. 8));
			--Ada.Text_IO.Put_Line("I got: " & Float'Image(X));
			--Y := Float'Value(dataString(9 .. 16));
			--Ada.Text_IO.Put_Line("I got: " & Float'Image(Y));
			--Z := Float'Value(dataString(17	.. 25));
			--Ada.Text_IO.Put_Line("I got: " & Float'Image(Z));

			--Send the data onward.
		exception
		        when Ada.IO_Exceptions.End_Error =>
		        null;
		end;
		
		--End the current connection and go back to wait for a new one.
		Ada.Text_IO.Put_Line("Ending connection...");
		GNAT.Sockets.Close_Socket (Connection);
		Ada.Text_IO.Put_Line("Connection ended");
	end loop;
end Echo_server;
