
# Auto Layout Graph

Auto layout of node graphs with nested groups

```cpp
int main(int argc, const char * argv[]) {
 
    ALGLayout layout = ALGLayout(25, 25);
    AutoLayoutGraph alg = AutoLayoutGraph(layout);
    
    try {
        
        ALGGroupNode* rootNode = new ALGGroupNode("root");
        
        ALGItemNode* aNode = new ALGItemNode("a", ALGSize(100, 100));
        alg.addToParent(rootNode, aNode);
        
        ALGItemNode* bNode = new ALGItemNode("b", ALGSize(100, 100));
        alg.addToParent(rootNode, bNode);
        
        ALGItemNode* cNode = new ALGItemNode("c", ALGSize(100, 100));
        alg.addToParent(rootNode, cNode);
        
        alg.connectWire(aNode, bNode);
        
        alg.connectWire(aNode, cNode);

        ALGGroupNode* group1Node = new ALGGroupNode("group1");
        alg.addToParent(rootNode, group1Node);
        
        ALGItemNode* xNode = new ALGItemNode("x", ALGSize(100, 100));
        alg.addToParent(group1Node, xNode);
        
        ALGItemNode* yNode = new ALGItemNode("y", ALGSize(100, 100));
        alg.addToParent(group1Node, yNode);
        
        alg.connectWire(xNode, yNode);
        
    } catch (const std::exception& error) {
        
        cout << "error: " << error.what() << endl;
    }
    
    return 0;
}
```
