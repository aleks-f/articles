using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading.Tasks;

namespace DynamicTest
{
    class Program
    {
	static void Main(string[] args)
	{
        var stringList = new List<string>();
        Random random = new Random();
        for (int i = 0; i < 100000; ++i)
        {
            stringList.Add(random.NextDouble().ToString());
        }

        string[] strvec = stringList.ToArray();

        doDynamic(strvec);
	}

    private static void doDynamic(string[] strvec)
    {
        Stopwatch sw = new Stopwatch();
		sw.Start();
		foreach (string s in strvec)
		{
			dynamic var = s;

            Int64 i = Convert.ToInt64(Convert.ToDouble(var));

			double d = Convert.ToDouble(var);

			var = i;
			string str = Convert.ToString(var);

			var = d;
			str = var.ToString();
	    }
        sw.Stop();
        System.Console.WriteLine(sw.ElapsedMilliseconds);
    }
 }
}
