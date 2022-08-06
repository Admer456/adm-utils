
#pragma once

// TODO:
// - use indices instead of pointers to elements in NTreeNode (better for searching)
// - use a vector of indices instead of a linked list of pointers (better for searching)
// - NTree::NodeType looks redundant and could be a template parameter instead
// - write a Rect class so we can have quadtrees
// - add relinking functionality, which will only 
//   relink newly added elements and not touch the node hierarchy
// - touch grass

namespace adm
{
	// Non-copyable NTree node
	template<typename elementType, typename boundingVolumeType, size_t Dimensions>
	struct NTreeNode
	{
		using ForEachChildFn = void( NTreeNode* );
		using ForEachElementFn = void( elementType );
		using GetSubdividedVolumeForChildFn = boundingVolumeType( boundingVolumeType, size_t );

		static constexpr size_t Combinations = 1 << Dimensions;

		NTreeNode() = default;
		NTreeNode( const NTreeNode& node ) = delete;
		NTreeNode( NTreeNode&& node ) = default;
		NTreeNode& operator=( NTreeNode&& node ) = default;

		NTreeNode( const boundingVolumeType& boundingVolume )
			: boundingVolume( boundingVolume ), numElements( 0 )
		{
		}

		NTreeNode( const boundingVolumeType& boundingVolume, const Vector<elementType>& elementList )
			: boundingVolume( boundingVolume )
		{
			numElements = elementList.size();
			for ( const auto& element : elementList )
			{
				elements.push_back( element );
			}
		}

		NTreeNode( const boundingVolumeType& boundingVolume, const LinkedList<elementType>& elementList )
			: boundingVolume( boundingVolume ), elements( elementList )
		{
			for ( const auto& element : elements )
			{
				numElements++;
			}
		}

		void AddElement( elementType element )
		{
			elements.push_back( element );
			numElements++;
		}

		void CreateChildren( LinkedList<NTreeNode<elementType, boundingVolumeType, Dimensions>>& octreeNodes, std::function<GetSubdividedVolumeForChildFn> getSubdividedVolumeForChild )
		{
			for ( uint32_t i = 0; i < Combinations; i++ )
			{
				auto& child = children[i];
				child = &octreeNodes.emplace_back( getSubdividedVolumeForChild( boundingVolume, i ) );
			}

			// Become a non-leaf because you have children now
			if ( IsLeaf() )
			{
				numElements *= -1;
			}
		}

		bool IsLeaf() const
		{
			return numElements > 0;
		}

		bool IsEmpty() const
		{
			return numElements == 0;
		}

		const boundingVolumeType& GetBoundingVolume() const
		{
			return boundingVolume;
		}

		int32_t GetNumElements() const
		{
			return numElements;
		}

		void ForEachChild( std::function<ForEachChildFn> function ) const
		{
			if ( IsLeaf() )
			{
				return;
			}

			for ( auto& child : children )
			{
				function( child );
			}
		}

		void ForEachElement( std::function<ForEachElementFn> function ) const
		{
			for ( auto& element : elements )
			{
				function( element );
			}
		}

	private:
		boundingVolumeType boundingVolume{};
		// > 0 -> leaf
		// = 0 -> empty
		// < 0 -> node with children
		int32_t numElements{ 0 };
		LinkedList<elementType> elements;

		NTreeNode* children[Combinations]{};
	};

	// Non-copyable quadtree node
	//template<typename elementType>
	//using QuadtreeNode = NTreeNode<elementType, Rect, 2>;

	// Non-copyable octree node
	template<typename elementType>
	using OctreeNode = NTreeNode<elementType, AABB, 3>;

	// Non-copyable N-dimensional tree designed to host static elements
	template<typename elementType, typename boundingVolumeType, size_t Dimensions>
	class NTree
	{
	public:
		using NodeType = NTreeNode<elementType*, boundingVolumeType, Dimensions>;
		static constexpr size_t Combinations = 1 << Dimensions;

		// Does the element intersect an AABB?
		using IntersectsBoxFn = bool( const elementType& element, const AABB& boundingVolume );
		// If this is a non-point, how much of it is inside this box?
		// Returned value can be any
		using BoxOccupancyFn = float( const elementType& element, const AABB& boundingVolume );
		// With these elements loaded, should this node subdivide any further?
		using ShouldSubdivideFn = bool( const NodeType& );
		// Get a subdivided bounding volume for the Nth child node
		using GetSubdividedVolumeForChildFn = boundingVolumeType( boundingVolumeType, size_t );

	public:
		NTree() = default;
		NTree( const NTree& octree ) = delete;
		NTree( NTree&& octree ) = default;
		NTree& operator=( NTree&& octree ) = default;

		NTree( const boundingVolumeType& volume, std::function<IntersectsBoxFn> intersectsBoxFunction,
			std::function<BoxOccupancyFn> boxOccupancyFunction,
			std::function<ShouldSubdivideFn> shouldSubdivideFunction,
			std::function<GetSubdividedVolumeForChildFn> getSubdividedVolumeForChildFunction )
		{
			Initialise( volume, intersectsBoxFunction, boxOccupancyFunction, shouldSubdivideFunction, getSubdividedVolumeForChildFunction );
		}

		void Initialise( const boundingVolumeType& volume, std::function<IntersectsBoxFn> intersectsBoxFunction,
			std::function<BoxOccupancyFn> boxOccupancyFunction,
			std::function<ShouldSubdivideFn> shouldSubdivideFunction,
			std::function<GetSubdividedVolumeForChildFn> getSubdividedVolumeForChildFunction )
		{
			boundingVolume = volume;
			intersectsBox = intersectsBoxFunction;
			occupiesBox = boxOccupancyFunction;
			shouldSubdivide = shouldSubdivideFunction;
			getSubdividedVolumeForChild = getSubdividedVolumeForChildFunction;
		}

		// Add a single element into the tree
		void AddElement( const elementType& element )
		{
			elements.push_back( element );
		}

		// Add elements into the tree
		void AddElements( const Vector<elementType>& elementList )
		{
			elements.insert( elements.end(), elementList.begin(), elementList.end() );
		}

		// Move the elements into the tree
		void SetElements( Vector<elementType>&& elementList )
		{
			elements = std::move( elementList );
		}

		// Recursively build octree nodes
		void BuildNode( NodeType* node )
		{
			// Node is a leaf, bail out
			if ( !shouldSubdivide( *node ) )
			{
				return;
			}

			// The node can be subdivided, create the child nodes
			// and figure out which element belongs to which node
			node->CreateChildren( nodes, getSubdividedVolumeForChild );
			node->ForEachElement( [&]( elementType* element )
				{
					// If the element is non-point and intersects with multiple
					// nodes, determine which one it'll ultimately belong to
					Vector<NodeType*> intersectingNodes;
					intersectingNodes.reserve( Combinations );
					node->ForEachChild( [&]( NodeType* child )
						{
							if ( intersectsBox( *element, child->GetBoundingVolume() ) )
							{
								intersectingNodes.push_back( child );
							}
						} );
					// No intersections at all, bail out
					if ( intersectingNodes.empty() )
					{
						return;
					}

					// It is only in one node, or an occupancy function
					// wasn't provided, don't bother checking spatial occupancy
					if ( intersectingNodes.size() == 1U || !occupiesBox )
					{
						intersectingNodes.front()->AddElement( element );
						return;
					}

					// Calculate surface area or volume inside each node
					NodeType* belongingNode = intersectingNodes.front();
					float maxOccupancy = -99999.0f;
					for ( auto& intersectingNode : intersectingNodes )
					{
						float occupancy = occupiesBox( *element, intersectingNode->GetBoundingVolume() );
						if ( occupancy > maxOccupancy )
						{
							belongingNode = intersectingNode;
							maxOccupancy = occupancy;
						}
					}

					// Finally, add the thing
					belongingNode->AddElement( element );
				} );

			// Now that we've done the heavy work, go down the tree
			node->ForEachChild( [&]( NodeType* child )
				{
					BuildNode( child );
				} );
		}

		// Rebuild the tree
		void Rebuild()
		{
			// Clear the tree and put the root node in
			leaves.clear();
			nodes.clear();
			auto& node = nodes.emplace_back( boundingVolume );

			// No elements, root node is empty
			if ( elements.empty() )
			{
				return;
			}

			// Fill it with all elements
			for ( auto& element : elements )
			{
				if ( intersectsBox( element, boundingVolume ) )
				{
					node.AddElement( &element );
				}
			}

			// Recursively subdivide the tree
			BuildNode( &node );

			// Now that the tree is built, find all leaf nodes
			for ( auto& node : nodes )
			{
				if ( node.IsLeaf() )
				{
					leaves.push_back( &node );
				}
			}
		}

	public: // Some getters'n'stuff
		const Vector<elementType>& GetElements() const
		{
			return elements;
		}

		const boundingVolumeType& GetBoundingVolume() const
		{
			return boundingVolume;
		}

		const LinkedList<NodeType>& GetNodes() const
		{
			return nodes;
		}

		const Vector<NodeType*>& GetLeaves() const
		{
			return leaves;
		}

	private:
		// The total bounding volume, equivalent to the bounding volume of the root
		boundingVolumeType boundingVolume;
		// Function that returns whether or not an element intersects the bounding volume of a node
		std::function<IntersectsBoxFn> intersectsBox;
		// Function that returns how much of the element is occupied by the bounding volume
		std::function<BoxOccupancyFn> occupiesBox;
		// Function that does the subdivision heuristic
		std::function<ShouldSubdivideFn> shouldSubdivide;
		// Function that subdivides the bounding volume of the parent node for the Nth child node
		std::function<GetSubdividedVolumeForChildFn> getSubdividedVolumeForChild;
		// The elements of this tree
		Vector<elementType> elements;
		// A linked list of nodes, contains the root node, its child nodes, child nodes of child nodes etc.
		LinkedList<NodeType> nodes;
		// A list of references to nodes that have no children
		Vector<NodeType*> leaves;
	};

	// Non-copyable quadtree designed to host static elements
	//template<typename elementType>
	//using Quadtree = NTree<elementType, Rect, 2>;

	// Non-copyable octree designed to host static elements
	template<typename elementType>
	using Octree = NTree<elementType, AABB, 3>;

	// In this namespace are utility functions when you construct one of the
	// more specific tree classes, like Octree
	// Example: Octree( bbox, IntersectsAABB, OccupiesBox, SimpleThreshold<Vec3, 100>, GetAABBForChild );
	// It gets a little wordy, but at least it's pretty modular and you can implement your own functions
	// for subdivision, intersection etc.
	namespace utils
	{
		// For Octree::intersectsVolume
		inline bool IntersectsAABB( const Vec3& element, const AABB& bbox )
		{
			return bbox.IsInside( element );
		}

		// For Octree::shouldSubdivide
		template<typename elementType, int Threshold>
		inline bool SimpleThreshold( const OctreeNode<elementType*>& node )
		{
			return node.GetNumElements() > Threshold;
		}

		// For Octree::occupiesBox
		inline float OccupiesBox( const Vec3& element, const AABB& bbox )
		{
			return 1.0f;
		}

		// For Octree::getSubdividedBoundingVolumeForChild
		inline AABB GetAABBForChild( AABB parentBbox, size_t i )
		{
			const Vec3 sizes[]
			{
				parentBbox.mins, parentBbox.maxs
			};

			// How to interpret this:
			// 000 -> mins.x, mins.y, mins.z
			// 010 -> mins.x, maxs.y, mins.z
			// The number basically means which extent to take the component from
			constexpr int sizeIndices[]
			{
				0, 0, 0,
				0, 0, 1,
				0, 1, 0,
				0, 1, 1,
				1, 0, 0,
				1, 0, 1,
				1, 1, 0,
				1, 1, 1,
			};

			const Vec3 centre = parentBbox.GetCentre();
			const Vec3 extent
			{
				sizes[sizeIndices[i * 3 + 0]].x,
				sizes[sizeIndices[i * 3 + 1]].y,
				sizes[sizeIndices[i * 3 + 2]].z
			};

			// AABB will be swapped if it's inverted, so worry not
			return AABB( centre, extent );
		}
	}
}
