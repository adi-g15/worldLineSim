* As of now, if we delete parent world node from GUI, it will work, but aside from this giving error at destruction of child,
	this also will be problematic for time travel, as copies are not made for that, so, when a parent node is deleted, assign the direct child all of the previous state, before the child's start, and assign it as another tree, in mutiverse_forest, since it's root node now
	All this will likely be done in WorldNode::end_simulation()

* Snake::grow and move don't modify tail

*Many entities, the new ones, may unknowingly not call addentity() on the box they are present,
* Use a consistent display 'API', which allows many options for display to be chosen from
