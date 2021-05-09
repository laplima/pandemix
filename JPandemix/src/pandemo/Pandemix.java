package pandemo;

import javax.swing.

public class Pandemix {
    public static void main(String [] args) {
        System.out.println("PANDEMIX");
        Person p = new Person();
        System.out.println(p.location);
        Location loc = new Location(10.0f, 20.0f);
        p.move(-5.0f, 8.0f);
        System.out.println(p.location);
    }
}
