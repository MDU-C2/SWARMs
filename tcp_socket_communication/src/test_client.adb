with Ada.Streams;
with Ada.Text_IO;

with GNAT.Sockets;

procedure test_client is
        use GNAT.Sockets;
        Address, From : Sock_Addr_Type;
        Socket : Socket_Type;
        Last : Ada.Streams.Stream_Element_Offset;

        --Data : String := "{""id"": 2, ""name"": ""abc""}";

        Data_Stream : Ada.Streams.Stream_Element_Array (1 .. 512);--Ada.Streams.Stream_Element_Offset(Data'Length));
begin
        --for I in Data_Stream'Range loop
          --      Data_Stream(I) := Ada.Streams.Stream_Element(Character'Pos(Data(Integer(I))));
                --Ada.Text_IO.Put(Character'Val(Data_Stream(I)));
        --end loop;
        Address.Port := 12345;
        Address.Addr := Any_Inet_Addr;
        Create_Socket (Socket, Family_Inet, Socket_Datagram);
        Bind_Socket(Socket, Address);
        --Connect_Socket (Socket, Address); 
        --Data_Stream := (others => 42);
				--delay 1.0;
        GNAT.Sockets.Receive_Socket (Socket, Data_Stream, Last, From);
        for I in 1 .. Last loop
               Ada.Text_IO.Put(Character'Val(Data_Stream(I)));
        end loop;
        --Ada.Text_IO.Put_Line ("Data: " & Data(1 .. Last));
        --Ada.Text_IO.Put_Line ("last : " & Last'Img);
        --Ada.Text_IO.Put_Line ("from : " & Image (From.Addr));

        Ada.Text_IO.Put_Line ("last :" & Last'Img);

end test_client;

