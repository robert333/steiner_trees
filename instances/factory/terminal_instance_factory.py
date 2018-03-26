import json
import numpy


def create_multiway_cut_dual_instance(size):
    simplex_size = 3 * size

    def is_inner_key_valid(key):
        assert len(key) == 3
        return all(0 <= i <= 2 * size - 1 for i in key)

    def is_inner_upper_node(key):
        assert is_inner_key_valid(key)
        return sum(key) == simplex_size - 1

    def is_inner_lower_node(key):
        assert is_inner_key_valid(key)
        return sum(key) == simplex_size - 2

    def compute_inner_neighbours(key):
        neighbours = []

        for i in range(3):
            neighbour_key = key.copy()
            neighbour_key[i] += -1 if is_inner_upper_node(key) else 1

            if is_inner_key_valid(neighbour_key):
                if is_inner_upper_node(key):
                    assert is_inner_lower_node(neighbour_key)
                else:
                    assert is_inner_upper_node(neighbour_key)
                neighbours.append(neighbour_key)

        return neighbours

    nodes = []
    node_id = 0

    # terminal nodes
    for i in range(3):
        nodes.append({"id": node_id})
        node_id += 1

    inner_node_map = {}
    inner_node_keys = []

    def get_key_string(key):
        return str(key[0]) + "," + str(key[1]) + "," + str(key[2])

    def add_node(key, node_id):
        nodes.append({"id": node_id})
        inner_node_map[get_key_string(key)] = node_id
        inner_node_keys.append(key)

    def get_node_id(key):
        return inner_node_map[get_key_string(key)]

    # inner upper nodes
    for i in range(simplex_size):
        for j in range(simplex_size - i):
            k = simplex_size - 1 - i - j
            key = [i, j, k]
            if is_inner_key_valid(key):
                add_node(key, node_id)
                node_id += 1

    # inner lower nodes
    for i in range(simplex_size - 1):
        for j in range(simplex_size - 1 - i):
            k = simplex_size - 2 - i - j
            key = [i, j, k]
            if is_inner_key_valid(key):
                add_node(key, node_id)
                node_id += 1

    edges = []

    # inner edges
    for key in inner_node_keys:
        if not is_inner_upper_node(key):
            continue
        for neighbour in compute_inner_neighbours(key):
            edges.append(
                {
                    "tail" : get_node_id(key),
                    "head" : get_node_id(neighbour),
                    "weight" : 1
                }
            )

    # terminal edges
    for t in range(3):
        for i in range(simplex_size):
            j = simplex_size - 1 - i
            neighbour = [0, 0, 0]
            neighbour[t] = 0
            neighbour[(t + 1) % 3] = i
            neighbour[(t + 2) % 3] = j

            if is_inner_key_valid(neighbour):
                assert is_inner_upper_node(neighbour)

                edges.append(
                    {
                        "tail" : t,
                        "head" : get_node_id(neighbour),
                        "weight" : 2 * (size + 1)
                    }
                )

        for i in range(simplex_size - 2 - (2 * size - 1) + 1):
            j = simplex_size - 2 - (2 * size - 1) - i
            neighbour = [0, 0, 0]
            neighbour[(t + 0) % 3] = i
            neighbour[(t + 1) % 3] = 2 * size - 1
            neighbour[(t + 2) % 3] = j

            if is_inner_key_valid(neighbour):
                assert is_inner_lower_node(neighbour)

                edges.append(
                    {
                        "tail" : t,
                        "head" : get_node_id(neighbour),
                        "weight" : 2 * (size + 1) + 1 + 2 * i
                    }
                )

        for i in range(simplex_size - 2 - (2 * size - 1) + 1):
            j = simplex_size - 2 - (2 * size - 1) - i
            neighbour = [0, 0, 0]
            neighbour[(t + 0) % 3] = i
            neighbour[(t + 1) % 3] = j
            neighbour[(t + 2) % 3] = 2 * size - 1

            if is_inner_key_valid(neighbour):
                assert is_inner_lower_node(neighbour)

                edges.append(
                    {
                        "tail" : t,
                        "head" : get_node_id(neighbour),
                        "weight" : 2 * (size + 1) + 1 + 2 * i
                    }
                )


    return {
        "graph": {
            "directed": False,
            "nodes": nodes,
            "edges": edges
        },
        "nets" : [
            {"name": "net_0", "terminals": [2, 1, 0]}
        ]
    }

instance = create_multiway_cut_dual_instance(3)

json.dump(instance, open("/home/robert/Documents/studies/discrete_mathematics/steiner-trees/instances/factory/factory_instance_01/instance.json", "w"), indent=4)