/**
 * Created by ericpts on 10/23/16.
 */
public class Derived extends Base {
    public Derived(Integer x) {
        super(x);
    }

    @Override
    public String toString() {
        return "Derived-" + i.toString();
    }
}
