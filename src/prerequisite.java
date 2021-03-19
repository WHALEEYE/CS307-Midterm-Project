public class prerequisite {
    private int id;
    private int courseId;
    private int[] requisiteCourseId;

    public prerequisite(int id, int courseId, int[] requisiteCourseId) {
        this.id = id;
        this.courseId = courseId;
        this.requisiteCourseId = requisiteCourseId;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getCourseId() {
        return courseId;
    }

    public void setCourseId(int courseId) {
        this.courseId = courseId;
    }

    public int[] getRequisiteCourseId() {
        return requisiteCourseId;
    }

    public void setRequisiteCourseId(int[] requisiteCourseId) {
        this.requisiteCourseId = requisiteCourseId;
    }
}
