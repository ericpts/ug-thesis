import jdk.internal.org.objectweb.asm.tree.analysis.Value;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

/**
 * Created by ericpts on 10/23/16.
 */
public class queue<ValueType>  {
    private List<ValueType> _data;

    public queue() {
        _data = new LinkedList<ValueType>();
    }

    public boolean add(ValueType e) {
        return _data.add(e);
    }

    public ValueType poll() {

        if (_data.isEmpty())
            return null;

        ValueType ret = _data.get(0);
        _data.remove(0);
        return ret;
    }

    public ValueType remove() {
        ValueType ret = _data.get(0);
        _data.remove(0);
        return ret;
    }

    public Iterator iterator() {
        return new queueIterator(_data);
    }
}

class queueIterator<ValueType> implements Iterator {
    private int cur = 0;
    private List<ValueType> _list;

    queueIterator(List<ValueType> list) {
        _list = list;
    }

    public ValueType next() {
        return _list.get(cur++);
    }

    public boolean hasNext() {
        return cur < _list.size();
    }
}
