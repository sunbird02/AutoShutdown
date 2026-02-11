using System;
using System.Windows.Forms;
using System.Diagnostics;
using System.Runtime.InteropServices;

public class ShutdownTool : Form
{
    private Label titleLabel;
    private Label delayLabel;
    private TextBox delayInput;
    private Button sleepBtn;
    private Button shutdownBtn;
    private Button cancelBtn;
    private Label statusLabel;
    private Label countdownLabel;
    private Timer countdownTimer;
    private int remainingSeconds = 0;
    private string actionType = "";

    [DllImport("PowrProf.dll", SetLastError = true)]
    private static extern bool SetSuspendState(bool hibernate, bool forceCritical, bool disableWakeupTimers);

    public ShutdownTool()
    {
        InitializeComponent();
    }

    private void InitializeComponent()
    {
        this.Text = "待机/关机工具";
        this.Width = 320;
        this.Height = 320;
        this.StartPosition = FormStartPosition.CenterScreen;
        this.FormBorderStyle = FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;
        this.MinimizeBox = false;
        this.BackColor = System.Drawing.Color.White;

        titleLabel = new Label();
        titleLabel.Text = "待机/关机工具";
        titleLabel.Font = new System.Drawing.Font("微软雅黑", 14, System.Drawing.FontStyle.Bold);
        titleLabel.Location = new System.Drawing.Point(20, 15);
        titleLabel.Size = new System.Drawing.Size(280, 30);
        this.Controls.Add(titleLabel);

        delayLabel = new Label();
        delayLabel.Text = "延迟时间 (时:分:秒):";
        delayLabel.Location = new System.Drawing.Point(20, 55);
        delayLabel.Size = new System.Drawing.Size(140, 25);
        this.Controls.Add(delayLabel);

        delayInput = new TextBox();
        delayInput.Text = "0:0:0";
        delayInput.Location = new System.Drawing.Point(160, 55);
        delayInput.Size = new System.Drawing.Size(140, 25);
        this.Controls.Add(delayInput);

        sleepBtn = new Button();
        sleepBtn.Text = "待机";
        sleepBtn.Location = new System.Drawing.Point(20, 95);
        sleepBtn.Size = new System.Drawing.Size(80, 35);
        sleepBtn.Click += SleepBtn_Click;
        this.Controls.Add(sleepBtn);

        shutdownBtn = new Button();
        shutdownBtn.Text = "关机";
        shutdownBtn.Location = new System.Drawing.Point(110, 95);
        shutdownBtn.Size = new System.Drawing.Size(80, 35);
        shutdownBtn.Click += ShutdownBtn_Click;
        this.Controls.Add(shutdownBtn);

        cancelBtn = new Button();
        cancelBtn.Text = "取消";
        cancelBtn.Location = new System.Drawing.Point(200, 95);
        cancelBtn.Size = new System.Drawing.Size(100, 35);
        cancelBtn.Click += CancelBtn_Click;
        this.Controls.Add(cancelBtn);

        countdownLabel = new Label();
        countdownLabel.Text = "";
        countdownLabel.Font = new System.Drawing.Font("微软雅黑", 24, System.Drawing.FontStyle.Bold);
        countdownLabel.Location = new System.Drawing.Point(20, 145);
        countdownLabel.Size = new System.Drawing.Size(280, 50);
        countdownLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
        countdownLabel.ForeColor = System.Drawing.Color.Red;
        this.Controls.Add(countdownLabel);

        statusLabel = new Label();
        statusLabel.Text = "";
        statusLabel.Location = new System.Drawing.Point(20, 205);
        statusLabel.Size = new System.Drawing.Size(280, 80);
        statusLabel.AutoSize = false;
        statusLabel.ForeColor = System.Drawing.Color.Green;
        this.Controls.Add(statusLabel);

        countdownTimer = new Timer();
        countdownTimer.Interval = 1000;
        countdownTimer.Tick += CountdownTimer_Tick;
    }

    private int ParseTimeInput(string input)
    {
        try
        {
            string[] parts = input.Split(':');
            if (parts.Length != 3) return -1;

            int hours = int.Parse(parts[0]);
            int minutes = int.Parse(parts[1]);
            int seconds = int.Parse(parts[2]);

            if (hours < 0 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59)
                return -1;

            return hours * 3600 + minutes * 60 + seconds;
        }
        catch
        {
            return -1;
        }
    }

    private string FormatTime(int totalSeconds)
    {
        int hours = totalSeconds / 3600;
        int minutes = (totalSeconds % 3600) / 60;
        int seconds = totalSeconds % 60;
        return $"{hours:D2}:{minutes:D2}:{seconds:D2}";
    }

    private void CountdownTimer_Tick(object sender, EventArgs e)
    {
        remainingSeconds--;
        countdownLabel.Text = FormatTime(remainingSeconds);

        if (remainingSeconds <= 0)
        {
            countdownTimer.Stop();
            if (actionType == "sleep")
            {
                SetSuspendState(false, true, true);
            }
            else if (actionType == "shutdown")
            {
                Process.Start("shutdown", "/s /t 0");
            }
        }
    }

    private void SleepBtn_Click(object sender, EventArgs e)
    {
        int delay = ParseTimeInput(delayInput.Text);
        if (delay >= 0)
        {
            if (delay > 0)
            {
                remainingSeconds = delay;
                actionType = "sleep";
                statusLabel.Text = "待机倒计时中...";
                statusLabel.ForeColor = System.Drawing.Color.Blue;
                countdownLabel.Text = FormatTime(remainingSeconds);
                countdownTimer.Start();
            }
            else
            {
                SetSuspendState(false, true, true);
            }
        }
        else
        {
            statusLabel.Text = "格式错误，请输入 时:分:秒\n例如: 0:5:30";
            statusLabel.ForeColor = System.Drawing.Color.Red;
            countdownLabel.Text = "";
        }
    }

    private void ShutdownBtn_Click(object sender, EventArgs e)
    {
        int delay = ParseTimeInput(delayInput.Text);
        if (delay >= 0)
        {
            try
            {
                if (delay > 0)
                {
                    remainingSeconds = delay;
                    actionType = "shutdown";
                    statusLabel.Text = "关机倒计时中...";
                    statusLabel.ForeColor = System.Drawing.Color.Blue;
                    countdownLabel.Text = FormatTime(remainingSeconds);
                    countdownTimer.Start();
                }
                else
                {
                    Process.Start("shutdown", "/s /t 0");
                }
            }
            catch (Exception ex)
            {
                statusLabel.Text = $"错误: {ex.Message}";
                statusLabel.ForeColor = System.Drawing.Color.Red;
            }
        }
        else
        {
            statusLabel.Text = "格式错误，请输入 时:分:秒\n例如: 0:5:30";
            statusLabel.ForeColor = System.Drawing.Color.Red;
            countdownLabel.Text = "";
        }
    }

    private void CancelBtn_Click(object sender, EventArgs e)
    {
        countdownTimer.Stop();
        countdownLabel.Text = "";
        try
        {
            Process.Start("shutdown", "/a");
            statusLabel.Text = "已取消待机/关机";
            statusLabel.ForeColor = System.Drawing.Color.Green;
        }
        catch
        {
            statusLabel.Text = "没有待机/关机任务";
            statusLabel.ForeColor = System.Drawing.Color.Orange;
        }
    }

    [STAThread]
    static void Main()
    {
        Application.EnableVisualStyles();
        Application.Run(new ShutdownTool());
    }
}
