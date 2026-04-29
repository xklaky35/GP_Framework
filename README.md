# GP-Framework - A little custom game engine


# Nodes
## Creating New Entities

A entity is a class that inherits from any base node and has additional logic attached to it.
If for example a player class inherits from the node class and implements own login, it is 
a entity.

To make a new entity work there are two steps to follow:

1. You need to inherit from a base node and implement the virtual methos from the Node baseclass:
    ```c++
    virtual void Init();
    virtual void Process(float deltaTime);
    virtual void SystemProcess();
    virtual void Draw(Renderer &);
    virtual void DrawDebug();
    ```
    These function don't have to be implemented but are needed if you want the entity to have custom behavior.


2. Each entity can be configured using a configuration file.
    It is loaded by creating a IniParser instance and specifying the files location.
    ```c++
    m_iniParser = new IniParser();
    m_iniParser->LoadIniFile("../game/scenes/whoosh/Player.ini");
    ```
    You can use the parser to get/set configurations and parameter from/to the file.
    
    Any child node you attach takes the parser as a parameter so that the configuraton of child nodes is saved
    for this entity.



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
                        if (ImGui::InputText("##Editor",  const_cast<char*>(s->m_pSpritePath), 28)) {
                            s->m_iniParser.SetValue("SpriteNode", "spritePath", s->m_pSpritePath);
                        }
                    }
                }
            }
        );
```
The parameter represents the current selected node in the node tree, so if you want to show information for a specific node,
you have to cast it.

As soon as the value is changed on the UI, the new value is saved in the iniFileParser, that will save the changed value to disk,
if a .ini file was specified in the node.