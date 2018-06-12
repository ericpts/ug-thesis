import java.lang.reflect.*;

public class Main {
  public int add(int a, int b)
  {
     return a + b;
  }

  public static void main(String args[])
  {
     try {
       Class cls = Class.forName("Main");
       Class partypes[] = new Class[2];
        partypes[0] = Integer.TYPE;
        partypes[1] = Integer.TYPE;
        Method meth = cls.getMethod(
          "add", partypes);
        Main methobj = new Main();
        Object arglist[] = new Object[2];
        arglist[0] = new Integer(37);
        arglist[1] = new Integer(47);
        Object retobj
          = meth.invoke(methobj, arglist);
        Integer retval = (Integer)retobj;
        System.out.println(retval.intValue());
     }
     catch (Throwable e) {
        System.err.println(e);
     }
  }
}
