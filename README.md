# Eric Stavarache's undergradute thesis

## Purpose
Optimize Java projects for size.

### Implementation
1. Parse Java .class and build a dependency tree.
2. Create a call-graph, starting from the main function.
3. Perform dead-method optimization, removing methods which are never called.

### Stretch goals
4. Remove unused classes entirely.
