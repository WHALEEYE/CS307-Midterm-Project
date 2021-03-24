import com.google.gson.*;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.sql.*;

public class connect {
    public static void main(String[] args) throws Exception {
        // 参数：
        // jdbc协议:postgresql子协议://主机地址:数据库端口号/要连接的数据库名
        String url = "jdbc:postgresql://121.4.163.50:10943/test_fei";
        // 数据库用户名
        String user = "postgres";
        // 数据库密码
        String password = "What1sth1s";

        // 1. 加载Driver类，Driver类对象将自动被注册到DriverManager类中
        java.lang.Class.forName("org.postgresql.Driver");
        // 2. 连接数据库，返回连接对象
        Connection conn = DriverManager.getConnection(url, user, password);

        try {
            //新建解析json文件的模块
            String dataPath = "F:\\数据库原理\\project\\project1\\src\\course_info.json";//配置解析的文本路径
            JsonParser parser = new JsonParser();  //创建JSON解析器
            BufferedReader in = new BufferedReader(new InputStreamReader(new FileInputStream(dataPath), StandardCharsets.UTF_8), 50 * 1024 * 1024); //设置缓冲区 编码
            JsonArray jsonArray = (JsonArray) parser.parse(in);  //创建JsonArray对象

            //程序化对数据库输入数据:  (int::n = 100只在测试阶段使用,后期换成json文件的长度)
            int n = jsonArray.size();
            insert_Course(jsonArray, conn, n);  //course表中数据的输入
            insert_Class(jsonArray, conn, n);   //class表中数据的输入
            insert_pre(jsonArray, conn, n); //先修课输入 转换成数字及符号
        } catch (JsonIOException | FileNotFoundException | JsonSyntaxException e) {
            e.printStackTrace();
        }
    }
    //course数据的输入封装
    private static void insert_Course(JsonArray jsonArray, Connection conn, int n) throws SQLException {
        for (int i = 0; i < n; i++) {
            //从json文件中读取对应序号的信息，创造course_info对象，准备后续输入数据库中
            course_info course_info = new course_info();
            jsonToCourse(jsonArray, i, course_info);
            //准备导入数据库所需的jdbc::PreparedStatement, 以及对应的sql语句
            String sql_addCourseIfNotExist = "INSERT INTO course(id, totalCapacity, courseId, courseHour, courseCredit, courseName, courseDept) VALUES(DEFAULT,?,?,?,?,?,?)" + "ON conflict(courseId)  DO NOTHING;";
            PreparedStatement ps_addCourse = conn.prepareStatement(sql_addCourseIfNotExist, Statement.RETURN_GENERATED_KEYS);
            //将course_info对象中的信息导入数据库中
            courseToDatabase(ps_addCourse, course_info);
        }
    }

    //class数据的输入封装
    private static void insert_Class(JsonArray jsonArray, Connection conn, int n) throws SQLException {
        for (int i = 0; i < n; i++) {
            //构造查询，辅助class_info对象数据输入
            String sql_get_courseId = "select id from course where coursename = ?";
            PreparedStatement ps_get_CourseId = conn.prepareStatement(sql_get_courseId);
            //从json文件中读取对应序号的信息，创造class_info对象，准备后续输入数据库中
            class_info class_info = new class_info();
            jsonToClass(jsonArray, ps_get_CourseId, i, class_info);
            //准备导入数据库所需的jdbc::PreparedStatement, 以及对应的sql语句
            String sql_add_Class = "INSERT INTO cLass(id, courseId, className, teacher) VALUES(DEFAULT, ?, ?, ?)";
            PreparedStatement ps_add_class = conn.prepareStatement(sql_add_Class);
            //将class_info对象中的信息导入数据库中
            classToDatabase(ps_add_class, class_info);
        }
    }
    private static void insert_pre(JsonArray jsonArray, Connection conn, int n) throws SQLException {
        for (int i = 0; i < n; i++) {
            prerequisite prerequisite = new prerequisite();
            //构造查询 根据courseName查询course的id
            String get_courseId = "select id from course where coursename = ?";
            PreparedStatement get_CourseId = conn.prepareStatement(get_courseId);
            jsonToPre(jsonArray, get_CourseId, i, prerequisite);


            //准备导入数据库所需的jdbc::PreparedStatement, 以及对应的sql语句（待完成）



            //将prerequisite对象中的信息导入数据库中（待完成）



        }
    }

    //以下是几个所用到的最基础组件：
    //将json文件中对应序号的数据读取并赋值给Java对象的组件
    private static void jsonToCourse(JsonArray jsonArray, int index, course_info course_info) {
        JsonObject jsonOBJ_courseArray = jsonArray.get(index).getAsJsonObject();
        course_info.setCourseId(jsonOBJ_courseArray.get("courseId").getAsString());
        course_info.setCourseCredit(jsonOBJ_courseArray.get("courseCredit").getAsInt());
        course_info.setTotalCapacity(jsonOBJ_courseArray.get("totalCapacity").getAsInt());
        course_info.setCourseHour(jsonOBJ_courseArray.get("courseHour").getAsInt());
        course_info.setCourseDept(jsonOBJ_courseArray.get("courseDept").getAsString());
        course_info.setCourseName(jsonOBJ_courseArray.get("courseName").getAsString());
    }
    private static void jsonToClass(JsonArray jsonArray, PreparedStatement ps_get_CourseId, int index, class_info class_info) throws SQLException {
        //通过json找出需要查询的string:::course_name
        JsonObject jsonOBJ_classArray = jsonArray.get(index).getAsJsonObject();
        String course_Name = jsonOBJ_classArray.get("courseName").getAsString();
        //由于标间外键的存在，插入之前先要查询对应course_Name的courseId
        ps_get_CourseId.setString(1, course_Name);
        ResultSet resultSet = ps_get_CourseId.executeQuery();
        resultSet.next();
        int courseId = resultSet.getInt("id");
            /*ResultSet resultId = addCourse.getGeneratedKeys();
            int id = 0;
            if (resultId.next()) {
                id = resultId.getInt(1);
            }*/
        //将class_info对象中的信息插入数据库
        class_info.setCourseId(courseId);
        class_info.setClassName(jsonOBJ_classArray.get("className").getAsString());
        if(jsonOBJ_classArray.get("teacher").equals(JsonNull.INSTANCE))
            class_info.setTeacher(null);
        else
            class_info.setTeacher(jsonOBJ_classArray.get("teacher").getAsString());
    }

    private static void jsonToPre(JsonArray jsonArray, PreparedStatement ps_get_pre, int index, prerequisite prerequisite) throws SQLException{
        JsonObject jsonOBJ_Pre = jsonArray.get(index).getAsJsonObject();
        String course_Name = jsonOBJ_Pre.get("courseName").getAsString();
        //由于标间外键的存在，插入之前先要查询对应course_Name的courseId
        ps_get_pre.setString(1, course_Name);
        ResultSet resultSet = ps_get_pre.executeQuery();
        resultSet.next();
        int courseId = resultSet.getInt("id");
        prerequisite.setCourseId(courseId);

        //获取原始prerequisite字符串
        String pre = jsonOBJ_Pre.get("prerequisite").getAsString();
        //转换
        if (pre != null){
            //以空格分割
            String[] arr = pre.split("\\s+");

            //给括号加了间距的新数组
            int ind = 0;   //tem数组每个时刻的位置
            String[] tem= new String[2 * arr.length];
            for (String value : arr) {
                if (value.charAt(0) == '(') {
                    tem[ind] = "(";
                    ind++;
                    tem[ind] = value.substring(1);
                } else if (value.charAt(value.length() - 1) == ')') {
                    int length = value.length();
                    tem[ind] = value.substring(0, length - 1);
                    ind++;
                    tem[ind] = ")";
                } else {
                    tem[ind] = value;
                }
                ind++;
            }

            //新字符串
            StringBuilder final_str = new StringBuilder();
            for (int i = 0; i < tem.length; i++) {
                if (tem[i] != null){
                    final_str.append(tem[i]);
                    if (i != tem.length - 1){
                        final_str.append(" ");
                    }
                }else {
                    break;
                }
            }

            //以空格分割
            String[] new_arr = final_str.toString().split("\\s+");

            //替换加入最终数组中
            String[] fin_arr = new String[new_arr.length];
            int t = 0;
            for(int i = 0; i < fin_arr.length; i++){
                switch (new_arr[i]) {
                    case "(", ")" -> fin_arr[t] = new_arr[i];

                    case "或者" -> fin_arr[t] = "+"; //或者用“+”替换

                    case "并且" -> fin_arr[t] = "*"; // "并且" -> "*"
                    default -> {
                        if (new_arr[i].equals("化学原理")){
                            if (i + 1 < new_arr.length && (new_arr[i + 1].equals("A") || new_arr[i + 1].equals("B"))){
                                fin_arr[t] = "化学原理 " + new_arr[i + 1];
                                i++;
                                ps_get_pre.setString(1, fin_arr[t]);
                            }else {
                                ps_get_pre.setString(1, new_arr[i]);
                            }
                        }
                        ResultSet result_Set = ps_get_pre.executeQuery();
                        resultSet.next();
                        int course_Id = resultSet.getInt("id");
                        fin_arr[t] = course_Id + "";
                    }
                }
                t++;
            }

            //生成替换后的字符串(形式为: ( 1 + 2) * 3 )
            StringBuilder final_Str = new StringBuilder();
            for (int i = 0; i < fin_arr.length; i++){
                if (fin_arr[i] != null){
                    if (i != fin_arr.length - 1){
                        final_Str.append(fin_arr[i]);
                        final_Str.append(" ");
                    }else {
                        final_Str.append(fin_arr[i]);
                    }
                }else {
                    break;
                }
            }
        }
    }
    //将java对象中存储的数据导入数据库的组件
    private static void courseToDatabase(PreparedStatement ps_addCourse, course_info course_info) throws SQLException {
        ps_addCourse.setInt(1, course_info.getTotalCapacity());
        ps_addCourse.setString(2, course_info.getCourseId());
        ps_addCourse.setInt(3, course_info.getCourseHour());
        ps_addCourse.setInt(4, course_info.getCourseCredit());
        ps_addCourse.setString(5, course_info.getCourseName());
        ps_addCourse.setString(6, course_info.getCourseDept());
        ps_addCourse.executeUpdate();
    }
    private static void classToDatabase(PreparedStatement ps_addClass, class_info class_info) throws SQLException {
        ps_addClass.setInt(1, class_info.getCourseId());
        ps_addClass.setString(2, class_info.getClassName());
        ps_addClass.setString(3, class_info.getTeacher());
        ps_addClass.executeUpdate();
    }
}