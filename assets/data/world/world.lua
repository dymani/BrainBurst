entityTemplates = {
    {
        name = "test1",
        components = {
            {
                type = "Physics",
                size = {1, 1},
                bodyType = "static"
            },
            {
                type = "Graphics",
                layer = "object",
                texture = "objects_HL06",
                textureRect = {0, 0, 64, 64},
                size = {1, 1}
            }
        }
    },
    {
        name = "test2",
        components = {
            {
                type = "Physics",
                size = {2, 3},
                bodyType = "dynamic"
            }
        }
    }
}