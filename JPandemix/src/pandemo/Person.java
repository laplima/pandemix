package pandemo;

// All classes implicitly inherit from Object
// Best practice: explicit @override annotation

class Location {
    public float x = 1.0f, y = 1.0f;
    public Location() {}
    public Location(float x, float y) {
        this.x = x;
        this.y = y;
    }
    @Override
    public String toString() {
        return "(" + x + "," + y + ")";
    }
}

enum HealthState { NONE, INFECTED, HEALED };

public class Person {
    public Location location = new Location();
    public HealthState state = HealthState.NONE;
    public void move(float dx, float dy) {
        location.x += dx;
        location.y += dy;
    }
    public boolean isSick() {
        return state == HealthState.INFECTED;
    }
    public boolean isImmune() {
        return state == HealthState.HEALED;
    }
}
