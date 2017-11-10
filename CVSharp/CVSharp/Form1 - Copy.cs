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

        CvCapture camera;
        CvMemStorage storage = new CvMemStorage();

        const int WIDTH = 640;
        const int HEIGHT = 480;
        const int PIX_P_FRAME = 240;
        const int BYTES_P_PIX = 4;


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


            IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 18004);
            UdpClient newsock = new UdpClient(ipep);

            IplImage image = new IplImage(new CvSize(WIDTH, HEIGHT), BitDepth.U8, BYTES_P_PIX);
            image.Set(new CvScalar(255, 255, 255, 255));
            //image.Flip(null, 0);
            
            Console.WriteLine("Waiting for a client...");

            IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);

            

        


            while (running)
            {
                
                UdpFrame frame = UdpFrame.Deserialize(newsock.Receive(ref sender));
                Int64 Offset = frame.SeqNo * PIX_P_FRAME * BYTES_P_PIX;
                Marshal.Copy(frame.Data, 0, new IntPtr(image.ImageData.ToInt64() + Offset), frame.Data.Length);



                if (frame.SeqNo == 0)
                    Console.WriteLine(frame.SeqNo);
                
                if (frame.SeqNo == 0)
                {
                    this.Invoke((MethodInvoker)delegate
                    {
                     //   image.Flip(null, 0);
                     //   image.Flip();
                        Bitmap Image = image.ToBitmap();
                    //    Image.RotateFlip(RotateFlipType.Rotate180FlipY);
                        pictureBox1.Image = Image;
                     //   image = new IplImage(new CvSize(640, 480), BitDepth.U8, 4);
                     //   GC.Collect();
                        
                    });
                }

            }
            image.Dispose();
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
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = PIX_P_FRAME)]
            public Int32[] Data;


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
