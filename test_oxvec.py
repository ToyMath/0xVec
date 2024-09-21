from oxvec import OxVec, CosineSimilarity, EuclideanDistance, DotProductSimilarity

# Initialize Database and Collection
oxvec = OxVec()
oxvec.create_collection("test_collection")
collection = oxvec.get_collection("test_collection")

# Add Vectors to Collection
collection.add_vector("vec1", [1.0, 0.0, 0.0])
collection.add_vector("vec2", [0.0, 1.0, 0.0])
collection.add_vector("vec3", [0.0, 0.0, 1.0])
collection.add_vector("vec4", [1.0, 1.0, 0.0])

# Update a Vector
collection.update_vector("vec1", [1.1, 0.0, 0.0])

# Delete a Vector
collection.delete_vector("vec2")

# Query Similar Vectors using Cosine Similarity
metric = CosineSimilarity()
query_vector = [1.0, 0.0, 0.0]
results = collection.query(query_vector, metric, 3)

# Print Results
print("Cosine Similarity Results:")
for entry in results:
    print(f"Vector ID: {entry.get_id()}, Vector: {entry.get_vector()}")

# Test Assertions
expected_ids = ["vec1", "vec4", "vec3"]  # Expected order based on cosine similarity
result_ids = [entry.get_id() for entry in results]
assert result_ids == expected_ids, "Cosine similarity results do not match expected order."

# Test Adding a New Vector
collection.add_vector("vec5", [1.0, 1.0, 1.0])
assert "vec5" in collection.vectors, "Failed to add new vector vec5."

# Test Updating a Vector
collection.update_vector("vec5", [1.1, 1.1, 1.1])
updated_vector = collection.get_vector("vec5").get_vector()
assert updated_vector == [1.1, 1.1, 1.1], "Failed to update vector vec5."

# Test Deleting a Vector
collection.delete_vector("vec5")
assert "vec5" not in collection.vectors, "Failed to delete vector vec5."

print("\nAll tests passed successfully.")
