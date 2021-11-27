using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using static System.IO.File;
using System.Drawing.Imaging;

namespace Metronome
{
    public partial class Form1 : System.Windows.Forms.Form
    {
        public delegate void AddDataDelegate(byte[] RxDataSet);
        public AddDataDelegate UIDelegate;
        public delegate void ChangeStatusDelegate(string status, Color status_color);
        public ChangeStatusDelegate StatusDelegate;

        int patternSwitch = 0;
        int[] _noteLen;
        float[] _barCmp;
        int _barLen;
        int _notePngIndex;
        byte[] RxData;
        string[] StrRx;
        byte[] RxChartData;
        byte[] Command = { 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        CheckBox[] _noteBoxes;
        Label[] _noteImages;
        Bitmap[] NotePng;
        
        public Form1()
        {
            InitializeComponent();
        }

        ~Form1()
        {
            serialPort.Close();
            serialPort.Dispose();
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            UInt16 bpm_val;
            UInt32 rythm_val;
            if (serialPort.IsOpen)
            {
                if (tBoxBpm.Text.Length == 0)
                {
                    MessageBox.Show("Please enter Bpm", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }
                bpm_val = Convert.ToUInt16(tBoxBpm.Text);
                if (bpm_val > 250 || bpm_val < 30)
                {
                    MessageBox.Show("Entered bpm value is not supported.\nTempo may very in range from 30 to 250 bpm", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                rythm_val = check_Rythm();
                if (rythm_val == 0)
                {
                    MessageBox.Show("Rythm is not selected.\n Please set the rythm", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }
                /* BPM formulation */
                Command[1] = (byte)(bpm_val >> 8);
                Command[2] = (byte)bpm_val;
                /* Note length formulation */
                Command[3] = (byte)Math.Pow(2, cBoxNoteLen.SelectedIndex);
                /* Rythm formulation */
                Command[4] = (byte)(rythm_val >> 24);
                Command[5] = (byte)(rythm_val >> 16);
                Command[6] = (byte)(rythm_val >> 8);
                Command[7] = (byte)rythm_val;

                serialPort.Write(Command, 0, 8);
            }
        }

        private UInt32 check_Rythm()
        {
            UInt32 counter = 0;
            for (int i = 1; i <= (int)(Math.Pow(2, cBoxNoteLen.SelectedIndex) * 4); i++)
                if (_noteBoxes[i-1].CheckState == CheckState.Checked)
                    counter += (UInt32)(0x1 << (32 - i));
            return counter;
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            this.UIDelegate = new AddDataDelegate(AddDataMethod);
            this.StatusDelegate = new ChangeStatusDelegate(SetStatusMethod);
            RxData = new byte[1025];
            StrRx = new string[1024];
            RxChartData = new byte[32];
            _noteLen = new int[32];
            _barCmp = new float[10];

            NotePng = new Bitmap[10];
            NotePng[0] = Metronome.Properties.Resources.whole;
            NotePng[1] = Metronome.Properties.Resources.dot_half;
            NotePng[2] = Metronome.Properties.Resources.half;
            NotePng[3] = Metronome.Properties.Resources.dot_forth;
            NotePng[4] = Metronome.Properties.Resources.forth;
            NotePng[5] = Metronome.Properties.Resources.dot_eigth;
            NotePng[6] = Metronome.Properties.Resources.eigth;
            NotePng[7] = Metronome.Properties.Resources.dot_sixteenth;
            NotePng[8] = Metronome.Properties.Resources.sixteenth;
            NotePng[9] = Metronome.Properties.Resources.thirtysecond;

            _noteBoxes = new CheckBox[32];
            _noteBoxes[0] = noteBox1;
            _noteBoxes[1] = noteBox2;
            _noteBoxes[2] = noteBox3;
            _noteBoxes[3] = noteBox4;
            _noteBoxes[4] = noteBox5;
            _noteBoxes[5] = noteBox6;
            _noteBoxes[6] = noteBox7;
            _noteBoxes[7] = noteBox8;
            _noteBoxes[8] = noteBox9;
            _noteBoxes[9] = noteBox10;
            _noteBoxes[10] = noteBox11;
            _noteBoxes[11] = noteBox12;
            _noteBoxes[12] = noteBox13;
            _noteBoxes[13] = noteBox14;
            _noteBoxes[14] = noteBox15;
            _noteBoxes[15] = noteBox16;
            _noteBoxes[16] = noteBox17;
            _noteBoxes[17] = noteBox18;
            _noteBoxes[18] = noteBox19;
            _noteBoxes[19] = noteBox20;
            _noteBoxes[20] = noteBox21;
            _noteBoxes[21] = noteBox22;
            _noteBoxes[22] = noteBox23;
            _noteBoxes[23] = noteBox24;
            _noteBoxes[24] = noteBox25;
            _noteBoxes[25] = noteBox26;
            _noteBoxes[26] = noteBox27;
            _noteBoxes[27] = noteBox28;
            _noteBoxes[28] = noteBox29;
            _noteBoxes[29] = noteBox30;
            _noteBoxes[30] = noteBox31;
            _noteBoxes[31] = noteBox32;

            _noteImages = new Label[32];
            _noteImages[0] = note1;
            _noteImages[1] = note2;
            _noteImages[2] = note3;
            _noteImages[3] = note4;
            _noteImages[4] = note5;
            _noteImages[5] = note6;
            _noteImages[6] = note7;
            _noteImages[7] = note8;
            _noteImages[8] = note9;
            _noteImages[9] = note10;
            _noteImages[10] = note11;
            _noteImages[11] = note12;
            _noteImages[12] = note13;
            _noteImages[13] = note14;
            _noteImages[14] = note15;
            _noteImages[15] = note16;
            _noteImages[16] = note17;
            _noteImages[17] = note18;
            _noteImages[18] = note19;
            _noteImages[19] = note20;
            _noteImages[20] = note21;
            _noteImages[21] = note22;
            _noteImages[22] = note23;
            _noteImages[23] = note24;
            _noteImages[24] = note25;
            _noteImages[25] = note26;
            _noteImages[26] = note27;
            _noteImages[27] = note28;
            _noteImages[28] = note29;
            _noteImages[29] = note30;
            _noteImages[30] = note31;
            _noteImages[31] = note32;

            for (int i = 0; i < 32; i++)
                _noteImages[i].BackColor = Color.Transparent;

            latency_lable.Text = "Latency: none";
            string[] ports = SerialPort.GetPortNames();
            cBoxCOMPort.Items.AddRange(ports);
            cBoxBAUDRate.SelectedIndex = cBoxBAUDRate.Items.Count - 1;
            cBoxNoteLen.SelectedIndex = 0;
            cBoxNoteLen.SelectedIndexChanged += SelectedIndexChanged;

            updateBarVariables();
            fillBarCmp();
            for (int i = _barLen; i < 32; i++)
                _noteBoxes[i].Enabled = false;
        }

        private void updateBarVariables()
        {
            _barLen = (int)(Math.Pow(2, cBoxNoteLen.SelectedIndex) * 4);
            _notePngIndex = (int)(cBoxNoteLen.SelectedIndex * 2 + 4);
            _notePngIndex = _notePngIndex > 9 ? 9 : _notePngIndex;
            fillBarCmp();
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort.DataBits = 8;
                serialPort.StopBits = StopBits.One;
                serialPort.Parity = Parity.None;
                serialPort.Handshake = Handshake.None;
                serialPort.WriteTimeout = 500;
                serialPort.ReadTimeout = 500;
                serialPort.PortName = cBoxCOMPort.Text;
                serialPort.BaudRate = Convert.ToInt32(cBoxBAUDRate.Text);

                serialPort.DataReceived += new SerialDataReceivedEventHandler(SerialPortRx_IRQh);
                serialPort.Disposed += SerialPort_Disposed;
                serialPort.Open();

                ConnectionStatus.Invoke(this.StatusDelegate, new Object[] { "Connected", Color.Green });
            }
            catch (Exception err)
            {
                ConnectionStatus.Invoke(this.StatusDelegate, new Object[] { "Error", Color.Red });
                MessageBox.Show(err.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void SerialPort_Disposed(object sender, EventArgs e)
        {
            ConnectionStatus.Invoke(this.StatusDelegate, new Object[] { "Disconnected", Color.Black });
        }

        private void SelectedIndexChanged(object cBox_sender, EventArgs e)
        {
            int i;
            updateBarVariables();
            for (i = 0; i < _barLen; i++)
                _noteBoxes[i].Enabled = true;
            for (; i < 32; i++)
            {
                _noteBoxes[i].Enabled = false;
                _noteBoxes[i].CheckState = CheckState.Unchecked;
                _noteImages[i].Image = null;
            }
            // samrt _noteLen & _noteImage update //
            for (int k = 0; k < _barLen; k++)
                if (_noteBoxes[k].Checked == true)
                    toggleNoteImage(k);
        }

        private void fillBarCmp()
        {
            _barCmp[0] = _barLen;
            _barCmp[1] = 3 * _barLen / (float)4;
            _barCmp[2] = _barLen / (float)2;
            _barCmp[3] = 3 * _barLen / (float)8;
            _barCmp[4] = _barLen / (float)4;
            _barCmp[5] = 3 * _barLen / (float)16;
            _barCmp[6] = _barLen / (float)8;
            _barCmp[7] = 3 * _barLen / (float)32;
            _barCmp[8] = _barLen / (float)16;
            _barCmp[9] = 1;
        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
                serialPort.Dispose();
            }
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            tBoxPortMonitor.Text = "";
        }

        private void SerialPortRx_IRQh(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            /* Redundancy check in case of driver errors */
            if (serialPort.IsOpen)
                if (serialPort.BytesToRead > 0)
                    try
                    {
                        sp.Read(RxData, 0, 1);
                        if (RxData[0] == 83)
                        {
                            while (serialPort.BytesToRead != 1024) ;
                            sp.Read(RxData, 1, 1024);
                        }   
                        else
                            sp.Read(RxData, 1, 7);
                        tBoxPortMonitor.Invoke(this.UIDelegate, new Object[] { RxData });
                    }
                    catch (TimeoutException exp)
                    {
                        ConnectionStatus.Invoke(this.StatusDelegate, new Object[] { "Package loss", Color.Red });
                        MessageBox.Show(exp.Message, "Received distored package", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        serialPort.Close();
                    }
        }
        public void AddDataMethod(byte[] RxDataSet)
        {
            if (RxData[0] == 76) // 'L'atency mode
            {
                latency_lable.Text = "Latency: " +  Convert.ToString(RxData[1] << 8 | RxData[2]) + "ms";
            }
            else if (RxData[0] == 83) // 'S'pectrum mode
            {
                for (int i = 1; i < 1025; i++)
                    StrRx[i-1] = Convert.ToString(RxData[i]);
                System.IO.File.WriteAllLines("Spectrum.txt", StrRx);
            }
            else // Tempo set answer
            {
                tBoxPortMonitor.Text += BitConverter.ToString(Command) + "\t:\t" + /*BitConverter.ToString(RxData)*/ Encoding.ASCII.GetString(RxData);
                tBoxPortMonitor.AppendText(Environment.NewLine);
            }
            Array.Clear(RxData, 0, RxData.Length);
        }
        public void SetStatusMethod(string status, Color status_color)
        {
            ConnectionStatus.ForeColor = status_color;
            ConnectionStatus.Text = status;
        }

        private void btnRhytmClear_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < (int)(Math.Pow(2, cBoxNoteLen.SelectedIndex) * 4); i++)
                _noteBoxes[i].CheckState = CheckState.Unchecked;
        }

        private void toggleNoteImage(int _noteIndex)
        {
            int next_note = _barLen;
            int prev_note = -1;
            calcNoteLength(_noteIndex, _noteBoxes[_noteIndex].Checked, ref next_note, ref prev_note);
            for (int i = prev_note + 1; i < next_note; i++)
                _noteImages[i].Image = null;  
            if (prev_note != -1)
            {
                setNoteImage(prev_note, false);
                if (_noteBoxes[_noteIndex].Checked)
                    suspendedSwitch(prev_note, _noteIndex - 1, _noteIndex);
                else
                    suspendedSwitch(prev_note, next_note - 1, next_note);
            }
            else if (_noteBoxes[0].Checked == false)
            {
                // Gost note in the beginnig of the bar //
                if (_noteBoxes[_noteIndex].Checked == false && next_note == _barLen)
                    _noteImages[0].Image = null;
                else
                {
                    int _gost_note = _noteBoxes[_noteIndex].Checked ? _noteIndex : next_note;
                    _noteLen[0] = _gost_note;
                    setNoteImage(0, true);
                    suspendedSwitch(0, _gost_note - 1, _gost_note);
                    _noteLen[0] = 0;
                }
            }
            if (_noteBoxes[_noteIndex].Checked)
            {
                setNoteImage(_noteIndex, false);
                suspendedSwitch(_noteIndex, next_note - 1, next_note);
            }
        }

        private void setNoteImage(int _noteIndex, bool gost)
        {
            for (int i = 0; i < 10; i++)
                if (_noteLen[_noteIndex] >= _barCmp[i])
                {
                    _noteImages[_noteIndex].Image = gost ? SetImageOpacity(NotePng[i], (float)0.5) : NotePng[i];
                    break;
                }
        }

        private void suspendedSwitch(int note_num, int _suspendedIndex, int next_note)
        {
            switch (_noteLen[note_num])
            {
                case 5:
                case 7:
                case 9:
                case 13:
                case 17:
                case 25:
                    _noteImages[_suspendedIndex].Image = SetImageOpacity(NotePng[_notePngIndex], (float)0.5);
                    break;
                case 10:
                case 14:
                case 18:
                case 26:
                    _noteImages[_suspendedIndex - 1].Image = SetImageOpacity(NotePng[_notePngIndex == 9 ? 8 : _notePngIndex - 2], (float)0.5);
                    break;
                case 11:
                case 15:
                case 19:
                case 27:
                    _noteImages[_suspendedIndex - 2].Image = SetImageOpacity(NotePng[_notePngIndex == 9 ? 7 : _notePngIndex - 3], (float)0.5);
                    break;
                case 20:
                case 28:
                    _noteImages[_suspendedIndex - 3].Image = SetImageOpacity(NotePng[6], (float)0.5);
                    break;
                case 22:
                case 30:
                    _noteImages[_suspendedIndex - 5].Image = SetImageOpacity(NotePng[5], (float)0.5);
                    break;
                case 21:
                case 29:
                    _noteImages[_suspendedIndex - 4].Image = SetImageOpacity(NotePng[6], (float)0.5);
                    _noteImages[_suspendedIndex].Image = SetImageOpacity(NotePng[9], (float)0.5);
                    break;
                case 23:
                case 31:
                    _noteImages[_suspendedIndex - 6].Image = SetImageOpacity(NotePng[5], (float)0.5);
                    _noteImages[_suspendedIndex].Image = SetImageOpacity(NotePng[9], (float)0.5);
                    break;
            }
        }

        private void calcNoteLength(int _noteIndex, bool _check, ref int next_note, ref int prev_note)
        {
            for (int i = _noteIndex + 1; i < _barLen; i++)
                if (_noteBoxes[i].Checked == true)
                {
                    next_note = i;
                    break;
                }
            for (int i = _noteIndex - 1; i >= 0; i--)
                if (_noteBoxes[i].Checked == true)
                {
                    prev_note = i;
                    break;
                }

            _noteLen[_noteIndex] = _check ? next_note - _noteIndex : 0;
            if (prev_note != -1)
                _noteLen[prev_note] = _check ? _noteIndex - prev_note : next_note - prev_note;
        }

        public Image SetImageOpacity(Image image, float opacity)
        {
            Bitmap bmp = new Bitmap(image.Width, image.Height);
            using (Graphics g = Graphics.FromImage(bmp))
            {
                ColorMatrix matrix = new ColorMatrix();
                matrix.Matrix33 = opacity;
                ImageAttributes attributes = new ImageAttributes();
                attributes.SetColorMatrix(matrix, ColorMatrixFlag.Default,
                                                  ColorAdjustType.Bitmap);
                g.DrawImage(image, new Rectangle(0, 0, bmp.Width, bmp.Height),
                                   0, 0, image.Width, image.Height,
                                   GraphicsUnit.Pixel, attributes);
            }
            return bmp;
        }
        private void noteBox1_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(0);
        }

        private void noteBox2_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(1);
        }

        private void noteBox3_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(2);
        }

        private void noteBox4_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(3);
        }

        private void noteBox5_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(4);
        }

        private void noteBox6_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(5);
        }

        private void noteBox7_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(6);
        }

        private void noteBox8_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(7);
        }

        private void noteBox9_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(8);
        }

        private void noteBox10_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(9);
        }

        private void noteBox11_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(10);
        }

        private void noteBox12_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(11);
        }

        private void noteBox13_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(12);
        }

        private void noteBox14_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(13);
        }

        private void noteBox15_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(14);
        }

        private void noteBox16_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(15);
        }

        private void noteBox17_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(16);
        }

        private void noteBox18_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(17);
        }

        private void noteBox19_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(18);
        }

        private void noteBox20_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(19);
        }

        private void noteBox21_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(20);
        }

        private void noteBox22_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(21);
        }

        private void noteBox23_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(22);
        }

        private void noteBox24_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(23);
        }

        private void noteBox25_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(24);
        }

        private void noteBox26_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(25);
        }

        private void noteBox27_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(26);
        }

        private void noteBox28_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(27);
        }

        private void noteBox29_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(28);
        }

        private void noteBox30_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(29);
        }

        private void noteBox31_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(30);
        }

        private void noteBox32_CheckedChanged(object sender, EventArgs e)
        {
            toggleNoteImage(31);
        }

        private void btnRP_Click(object sender, EventArgs e)
        {
            for (int k = 0; k < _barLen; k++)
                _noteBoxes[k].Checked = false;
            switch (patternSwitch)
            {
                case 0:
                    for (int k = 0; k < _barLen; k++)
                        _noteBoxes[k].Checked = true;
                    break;
                case 1:
                    for (int k = 0; k < _barLen; k += 2)
                        _noteBoxes[k].Checked = true;
                    break;
                case 2:
                    for (int k = 0; k < _barLen && _barLen > 4; k += 4)
                        _noteBoxes[k].Checked = true;
                    break;
                case 3:
                    for (int k = 0; k < _barLen && _barLen > 8; k += 8)
                        _noteBoxes[k].Checked = true;
                    break;
            }
            switch(_barLen)
            {
                case 4:
                    if (++patternSwitch > 1)
                        patternSwitch = 0;
                    return;
                case 8:
                    if (++patternSwitch > 2)
                        patternSwitch = 0;
                    return;
                default:
                    if (++patternSwitch > 3)
                        patternSwitch = 0;
                    return;
            } 
        }
    }
}
