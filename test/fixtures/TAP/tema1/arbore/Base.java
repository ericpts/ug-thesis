import java.util.Comparator;

/**
 * Created by ericpts on 10/23/16.
 */
public class Base implements Comparable {

    Integer i;

    public Base(Integer x) {
        i = x;
    }

    @Override
    public int compareTo(Object o) {
        Base co = (Base)o;
        return i.compareTo(co.getI());
    }

    public Integer getI() {
        return i;
    }

    public String toString() {
        return "Base-" + i.toString();
    }
}
