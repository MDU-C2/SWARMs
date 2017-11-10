using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Net;
using System.Net.Sockets;



using System.Runtime.InteropServices;

using OpenCvSharp;




namespace CVSharp
{



    public partial class Form1 : Form
    {
        Thread thread;

        //CvCapture camera;
        CvMemStorage storage = new CvMemStorage();

        const int WIDTH = 1920/4; //3840/8; //960; //1920; //320; //640;
        const int HEIGHT = 1080/4; //2748/8; //687; //1080; //240; //480;
        const int PIX_P_FRAME = 480;
        const int BYTES_P_PIX = 3;
        const int BYTES_P_FRAME = PIX_P_FRAME * BYTES_P_PIX;


        volatile bool running = true;

        public Form1()
        {
            InitializeComponent();

            thread = new Thread(new ThreadStart(CameraLoop));
            thread.Start();
        }


        int ClickX = -1;
        int ClickY = -1;


        public void CameraLoop()
        {




            IplImage image = new IplImage(new CvSize(WIDTH, HEIGHT), BitDepth.U8, BYTES_P_PIX);
            image.Set(new CvScalar(255, 255, 255, 255));
            //image.Flip(null, 0);
            IplImage image2 = new IplImage(new CvSize(WIDTH, HEIGHT), BitDepth.U8, 1);
            image2.Set(new CvScalar(255));

            IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 18004);

            //udp
            UdpClient newsock = new UdpClient(ipep);
            Console.WriteLine("Waiting for a client...");
            IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);

            //tcp
            //Socket newsock = new Socket(AddressFamily.InterNetwork,SocketType.Stream, ProtocolType.Tcp);
            //newsock.Bind(ipep);
            //newsock.Listen(10);
            //Console.WriteLine("Waiting for a client...");
            //Socket client = newsock.Accept();
            //IPEndPoint clientep =
            //             (IPEndPoint)client.RemoteEndPoint;
            //Console.WriteLine("Connected with {0} at port {1}",
            //                clientep.Address, clientep.Port);

            //byte[] data = new byte[8 + BYTES_P_PIX * PIX_P_FRAME];

            

        


            while (running)
            {

                UdpFrame frame = UdpFrame.Deserialize(newsock.Receive(ref sender));
                //client.Receive(data);
                //UdpFrame frame = UdpFrame.Deserialize(data);
                //Int64 Offset = frame.SeqNo * BYTES_P_FRAME;
                Int64 Offset = WIDTH*HEIGHT*BYTES_P_PIX - (frame.SeqNo+1) * BYTES_P_FRAME; //flip and mirror
                //Int64 Offset = 
                //Console.WriteLine("SeqNo: {0}",frame.SeqNo);
                //if (frame.CamNo == 2)
                //{
                //    Marshal.Copy(frame.Data, 0, new IntPtr(image2.ImageData.ToInt64() + Offset), frame.Data.Length);
                //    //if(frame.CamNo == 2)
                //    //    if (frame.SeqNo == 0)
                //    //        Console.WriteLine("gray new frame");
                //}
                //else 
                //{
                //    Marshal.Copy(frame.Data, 0, new IntPtr(image.ImageData.ToInt64() + Offset), frame.Data.Length);
                //}
                Marshal.Copy(frame.Data, 0, new IntPtr(image.ImageData.ToInt64() + Offset), frame.Data.Length);

                //if (frame.SeqNo == 0)
                //    Console.WriteLine(frame.CamNo);
                
                if (frame.SeqNo == 0)
                {
                    this.Invoke((MethodInvoker)delegate
                    {
                        //image.Flip(null, 0);
                        //image.Flip();
                        Bitmap Image;
                        if (frame.CamNo == 2)
                            Image = image2.ToBitmap();
                        else
                            Image = image.ToBitmap();
             //           Image.RotateFlip(RotateFlipType.Rotate180FlipY);
                        pictureBox1.Image = Image;
                     //   image = new IplImage(new CvSize(640, 480), BitDepth.U8, 4);
                     //   GC.Collect();
                        
                    });
                }

            }
            image.Dispose();
            image2.Dispose();
            GC.Collect();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            running = false;
            Thread.Sleep(1000);
            thread.Abort();

        }

        private void pictureBox1_MouseClick(object sender, MouseEventArgs e)
        {
            ClickX = e.X;
            ClickY = e.Y;
        }



        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct UdpFrame
        {
            public UInt32 CamNo;
            public UInt32 SeqNo;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = BYTES_P_FRAME)]
            public Byte[] Data;


            public static UdpFrame Deserialize(Byte[] data, int offset = 0)
            {
                int objsize = Marshal.SizeOf(typeof(UdpFrame));
                IntPtr buff = Marshal.AllocHGlobal(objsize);
                Marshal.Copy(data, offset, buff, objsize);
                UdpFrame retStruct = (UdpFrame)Marshal.PtrToStructure(buff, typeof(UdpFrame));
                Marshal.FreeHGlobal(buff);
                return retStruct;
            }
        }
    }
}
