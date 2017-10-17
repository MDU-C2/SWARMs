with Ada.Streams;
with Ada.Text_IO;

with GNAT.Sockets;

procedure udp_test_server is
   use GNAT.Sockets;
   Server : Socket_Type;
	 Receiver : Socket_Type;
	Connection : GNAT.Sockets.Socket_Type;
Client : GNAT.Sockets.Sock_Addr_Type;
Channel : GNAT.Sockets.Stream_Access;

   Address, From, To : Sock_Addr_Type;
   --Data : Ada.Streams.Stream_Element_Array (1 .. 512);
   Last : Ada.Streams.Stream_Element_Offset;
   Data : String := "Hello!";--"{""id"": 2, ""name"": ""abc""}";
   Data_Stream : Ada.Streams.Stream_Element_Array (1 .. Ada.Streams.Stream_Element_Offset(Data'Length));
   Watchdog : Natural := 0;
   Str : String(1..24) := (others => ASCII.NUL);
begin

						
-- inisialize the socket.
   Ada.Text_IO.Put_Line("Starting server");
   Ada.Text_IO.Put_Line("Creating socket");
   Create_Socket(Server, Family_Inet, Socket_Datagram);
   --GNAT.Sockets.Create_Socket (Socket => Receiver);
   --GNAT.Sockets.Set_Socket_Option
   --   (Socket => Receiver, Option => (Name    => GNAT.Sockets.Reuse_Address, Enabled => True));
   
   --Address.Addr := Any_Inet_Addr;
   --Address.Port := 12345;
   --Bind_Socket(Server, Address);
 
   Ada.Text_IO.Put_Line("Listening for connection...");
   --GNAT.Sockets.Listen_Socket (Server);

   Client.Addr := Any_Inet_Addr;
   Client.Port := 12345;
   --loop

    --Wait for a client to connect.
   Ada.Text_IO.Put_Line("Waiting for connection...");
   --GNAT.Sockets.Accept_Socket(Server, Socket  => Connection, Address => Client);
   Ada.Text_IO.Put_Line("Client connected from " & GNAT.Sockets.Image (Client));
   --Channel := GNAT.Sockets.Stream(Server);
  -- loop
   --  begin
       --String'Write(Channel, Data);
   loop
            for I in Data_Stream'Range loop
        --      Ada.Text_IO.Put(Character'Val(I));
              Data_Stream(I) := Ada.Streams.Stream_Element(Character'Pos(Data(Integer(I))));
              Ada.Text_IO.Put(Character'Val(Data_Stream(I)));
              
            end loop;
            delay 1.0;
            Send_Socket (Server, Data_Stream, Last, Client);
            Ada.Text_IO.Put_Line("Sending...");
   end loop;
            Close_Socket(Server);
            --exception
              --       when Socket_Error =>
                --          Watchdog := Watchdog + 1;
                  --        exit when Watchdog = 10;
		--	end;
   --  end loop;
--                        GNAT.Sockets.Receive_Socket (Server, Data, Last, From);
  ---                      for I in 1 .. Last loop
     ---                           Ada.Text_IO.Put(Character'Val(Data(I)));
        ---                end loop;
           --             --Ada.Text_IO.Put_Line ("Data: " & Data(1 .. Last));
             --           Ada.Text_IO.Put_Line ("last : " & Last'Img);
               --         Ada.Text_IO.Put_Line ("from : " & Image (From.Addr));
                --exception
              --          when Socket_Error =>
            --                    Watchdog := Watchdog + 1;
          --                      exit when Watchdog = 10;
        --         end;            
      --   end loop;
end udp_test_server; 
