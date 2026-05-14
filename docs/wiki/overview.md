
# Overall structure

![GPFrameworkOverview](docs/GPFrameworkOverview.pdf)



# Nodes
This engine uses small node tree structures that form scenes.

![UMLNodeStructure.pdf](docs/UMLNodeStructure.pdf)


## Creating New Custom Entities


A entity is a class that inherits from any base node and has additional logic attached to it.
If for example a player class inherits from the node class and implements its own login, it is 
a entity.

To make a new entity work there are two steps to follow:

1. You need to inherit from a base node and implement the virtual methods from the Node baseclass:
    ```c++
    virtual void Init();
    virtual void Process(float deltaTime);
    virtual void SystemProcess();
    virtual void Draw(Renderer &);
    virtual void DrawDebug();
    virtual void SetupParameter(IniParser* parser, string sectionId);
    ```
    These function don't have to be implemented but are needed if you want the entity to have custom behavior.



2. Now you have to register the node in the system. To do that, you first need to register the class using the [NodeFactory](#nodefactory).
    ```c++

    
    Player::Player() {
        
        // helper class from the NodeFactory class:
        // stores the class initializer for later access to the class
        REGISTER_CLASS(Player);
        
        // the name has to be the exact same as the classname
        // this is necessary for mapping the component loaded from a configuration file to a class
        SetupNode("Player", NT_Custom);
    }
    ```
    
3. Load the configuration file for this component. This is done in the SetupParameter(...) function. 
    
   ```c++
   void Player::SetupParameter(IniParser *parser, const std::string &section) {
   
       // IMORTANT!!
       // the parent node configures the inherited properties for you
       Node::SetupParameter(parser, section);
   
       // you only need to load the properties for this component
       m_maxJumps = parser->GetValueAsInt(section, "maxJumps");
       ...
   
       // if this component has child components attached, specify the path to the configuration file of this component
       NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Player.ini");
   }
   ```
After this setup, the component will be shown in the asset browser.
All configuration made to the component and its children will be saved on disk and are loaded once the engine starts.



## ImGui Node Editor

The editor can currently be toggled with tab.

The main thing here is the node tree and the corresponding node editor window.
If you select a node the node editor changes its contents to the current selected node 
configuration.
Changing there values (should) result in instant changes to the element currently active in the scene.

For this to be possible, each node has to follow the same setup:

Because every node inherits from the Node-Class, every node has the **m_nodeInfo** member.
This is a list of structs that contains information ImGui uses to render the information on the screen.
```c++
    struct NodeInfo
    {
        const char*     Name;             // Member name
        std::function<void(Node&)> Draw;  // Function that renders the info
    };
```

ImGui is calling the function and expects elements to be drawn in it.
The function has to contains the ImGui elements that should be rendered:
```c++
        m_nodeInfo.push_back({
            "SpritePath",   // name
            [](Node &n) {   // function
                    if (auto* s = dynamic_cast<SpriteNode*>(&n)) {
                        if (ImGui::InputText("##Editor",  spritePath, 28)) {
                            s->m_iniParser.SetValue(nodeId, "spritePath");
                        }
                    }
                }
            }
        );
```
The parameter represents the current selected node in the node tree, so if you want to show information for a specific node,
you have to cast it.

As soon as the value is changed on the UI, the new value is saved in the IniFileParser, that will save the changed value to disk,
if a .ini file for this node was found.