using System.IO.Ports;
using System.Net;
using System.Text;

namespace StormworksWeb;

public class Program
{
    private static int _numberOfRequests = 0;
    private static string _speedKmph = "0";
    private static string _speedMph = "0";
    private static string _bearing = "0";
    private static string _altitude = "0";
    private static SerialPort _serialPort = new SerialPort("COM3", 9600, Parity.None, 8, StopBits.One);

    public static void Main()
    {
        _serialPort.PortName = SetPortName("COM1");
        _serialPort.ReadTimeout = 500;
        _serialPort.WriteTimeout = 500;
        _serialPort.Open();

        var httpListener = new HttpListener();
        httpListener.Prefixes.Add(("http://localhost:8000/"));
        httpListener.Prefixes.Add(("http://127.0.0.1:8000/"));
        httpListener.Start();

        Console.WriteLine("Listening on port: 8000");

        while (true)
        {
            var context = httpListener.GetContext();
            HandleRequest(context);
        }
    }

    private static void HandleRequest(HttpListenerContext context)
    {
        var request = context.Request;
        var response = context.Response;
        _numberOfRequests++;

        _speedKmph = request.QueryString.Get("kmph") ?? _speedKmph;
        _speedMph = request.QueryString.Get("mph") ?? _speedMph;
        _bearing = request.QueryString.Get("bearing") ?? _bearing;
        _altitude = request.QueryString.Get("alt") ?? _altitude;

        var serialText = $"{_speedKmph}:{_speedMph}:{_altitude}:{_bearing}";
        _serialPort.WriteLine(serialText);
        Console.WriteLine(serialText);

        var responseString = $"<html>" +
                             $"<body>" +
                             $"<h1>Received {request.HttpMethod} request from {request.UserHostAddress}.</h1>" +
                             $"<h2>Request number {_numberOfRequests}</h2>" +
                             $"Last Speed (kmph): {_speedKmph}<br/>" +
                             $"Last Speed (mph): {_speedMph}<br/>" +
                             $"Altitude: {_altitude}<br/>" +
                             $"Bearing: {_bearing}<br/>" +
                             $"<script>window.setTimeout(function() {{ window.location.reload(); }}, 1000);</script>" +
                             $"</body>" +
                             $"</html>";
        var buffer = Encoding.UTF8.GetBytes(responseString);

        response.ContentLength64 = buffer.Length;
        var responseOutput = response.OutputStream;
        responseOutput.Write(buffer, 0, buffer.Length);
        responseOutput.Close();
    }

    // Display Port values and prompt user to enter a port.
    private static string SetPortName(string defaultPortName)
    {
        Console.WriteLine("Available Ports:");
        foreach (string s in SerialPort.GetPortNames())
        {
            Console.WriteLine("   {0}", s);
        }

        Console.Write("Enter COM port value (Default: {0}): ", defaultPortName);
        string portName = Console.ReadLine() ?? "";

        if (portName == "" || !(portName.ToLower()).StartsWith("com"))
        {
            portName = defaultPortName;
        }

        return portName;
    }
}