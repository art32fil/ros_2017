from visualization_msgs.msg import InteractiveMarkerControl, Marker


class Weapon:
    def __init__(self, parent):
        self.weapon = Marker()
        self.weapon.type = Marker.ARROW
        self.weapon.scale.x = self.int_marker.scale * 0.45
        self.weapon.scale.y = self.int_marker.scale * 0.45
        self.weapon.scale.z = self.int_marker.scale * 0.45
        self.weapon.color.r = 1.0 / float(randint(1, 5))
        self.weapon.color.g = 1.0 / float(randint(1, 5))
        self.weapon.color.b = 1.0 / float(randint(1, 5))
        self.weapon.color.a = 1.0

        self.control = InteractiveMarkerControl()
        self.control.markers.append(self.body)

        # self.int_marker.controls.append(self.control)

