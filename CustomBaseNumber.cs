public class CustomBaseNumber{
    public static string Increment(string char_list, string current){
        int[] index_array = current.ToCharArray().Select(d => char_list.IndexOf(d)).Reverse().ToArray();
        int index = 0;
        bool carry = false;
        do{
            if(++index_array[index] == char_list.Length){
                if((carry = (index_array[index++] = 0) == 0) && index == index_array.Count)
                    index_array.Add(0);
            }
            else
                carry = false;
        } while(carry);
        return new string(index_array.Select(d => char_list[d]).Reverse());
    }
}
