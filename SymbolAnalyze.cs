using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Text.RegularExpressions;

namespace SymbolAnalyze
{
    /// <summary>
    /// readelf -s -W libminecraftpe.so | c++filt
    /// ※ readelfはAndroidNDK(SDKじゃなくてNDKだよ！)のを使わないとうまく読めない
    /// </summary>
    public class SymbolAnalyze
    {
        public static void analyze(string file)
        {
            List<ClassDef> class_list = new List<ClassDef>();
            using (StreamReader reader = new StreamReader(file))
            {
                while (reader.Peek() != -1)
                {
                    string line = reader.ReadLine();
                    if (Regex.IsMatch(line, @"[ \t]+[0-9]+: [0-9a-f]+[ \t]+[0-9]+ (FUNC|OBJECT)[ \t]+(GLOBAL|WEAK)[ \t]+DEFAULT[ \t]+[0-9]+ [a-zA-Z0-9]+::.*"))
                    {
                        var m = Regex.Match(line, @"[ \t]+[0-9]+: (?<address>[0-9a-f]+)[ \t]+[0-9]+ (?<type>FUNC|OBJECT)[ \t]+(?<bind>GLOBAL|WEAK)[ \t]+DEFAULT[ \t]+[0-9]+ (?<ClassName>[a-zA-Z0-9]+)::(?<member>.*)");
                        ClassDef classdef = class_list.Find(d => d.class_name == m.Groups["ClassName"].Value);
                        if (classdef == null)
                        {
                            classdef = new ClassDef { class_name = m.Groups["ClassName"].Value };
                            class_list.Add(classdef);
                        }
                        classdef.member.Add(new ClassMember { address = m.Groups["address"].Value, member_type = m.Groups["type"].Value == "FUNC" ? MemberType.FUNC : MemberType.OBJECT, bind_type = m.Groups["bind"].Value == "GLOBAL" ? BindType.GLOBAL : BindType.WEAK, member = m.Groups["member"].Value });
                    }
                }
            }

            Directory.CreateDirectory("ClassList");
            foreach (var classdef in class_list)
            {
                Directory.CreateDirectory("ClassList\\" + classdef.class_name);
                if (classdef.member.Exists(d => d.member_type == MemberType.FUNC))
                {
                    using (StreamWriter writer = new StreamWriter("ClassList\\" + classdef.class_name + "\\FUNC.txt"))
                    {
                        foreach (var member in classdef.member.Where(d => d.member_type == MemberType.FUNC))
                        {
                            if (member.bind_type == BindType.GLOBAL)
                                writer.WriteLine("{0} GLOBAL  {1}", member.address, member.member);
                            else
                                writer.WriteLine("{0} WEAK    {1}", member.address, member.member);
                        }
                    }
                }
                if (classdef.member.Exists(d => d.member_type == MemberType.OBJECT))
                {
                    using (StreamWriter writer = new StreamWriter("ClassList\\" + classdef.class_name + "\\OBJECT.txt"))
                    {
                        foreach (var member in classdef.member.Where(d => d.member_type == MemberType.OBJECT))
                        {
                            if (member.bind_type == BindType.GLOBAL)
                                writer.WriteLine("{0} GLOBAL  {1}", member.address, member.member);
                            else
                                writer.WriteLine("{0} WEAK    {1}", member.address, member.member);
                        }
                    }
                }
            }
            Console.WriteLine();//ブレークポイント用
        }

        public enum MemberType { FUNC, OBJECT }
        public enum BindType { GLOBAL, WEAK }
        public class ClassMember
        {
            public string address { get; set; }
            public MemberType member_type { get; set; }
            public BindType bind_type { get; set; }
            public string member { get; set; }
        }

        public class ClassDef
        {
            public List<ClassMember> member = new List<ClassMember>();
            public string class_name { get; set; }
        }
    }
}
