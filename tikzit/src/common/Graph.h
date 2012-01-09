//
//  Graph.h
//  TikZiT
//  
//  Copyright 2010 Aleks Kissinger. All rights reserved.
//  
//  
//  This file is part of TikZiT.
//  
//  TikZiT is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  TikZiT is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with TikZiT.  If not, see <http://www.gnu.org/licenses/>.
//  


/*!
 
 @mainpage	TikZiT
			TikZiT is a GUI application for drawing, editing, and parsing TikZ
			diagrams. Common code is in src/common, and plaform-specific code is
			in src/{osx,linux}.
 
 */


#import "Node.h"
#import "Edge.h"
#import "GraphChange.h"
#import "BasicMapTable.h"
#import "Transformer.h"

/*!
 @class      Graph 
 @brief      Store a graph, output to TikZ.
 @details    All of the methods that change a graph return an object of type GraphChange.
			 Graph changes can be re-done by calling applyGraphChange. They can be undone
			 by calling applyGraphChange on [change inverse].
 */
@interface Graph : NSObject {
	NSLock *graphLock;
	BOOL dirty; // this bit keeps track of whether nodesCache and edgesCache are up to date
	NSMutableSet *nodes;
	NSMutableSet *edges;
	NSSet *nodesCache;
	NSSet *edgesCache;
	
	BasicMapTable *inEdges;
	BasicMapTable *outEdges;
	
	GraphElementData *data;
	NSRect boundingBox;
}

/*!
 @property   data
 @brief      Data associated with the graph.
 */
@property (copy) GraphElementData *data;

/*!
 @property   nodes
 @brief      The set of nodes.
 @details    The node set is cached internally, so no need to lock
             the graph when enumerating.
 */
@property (readonly) NSSet *nodes;

/*!
 @property   edges
 @brief      The set of edges.
 @details    The edge set is cached internally, so no need to lock
             the graph when enumerating.
 */
@property (readonly) NSSet *edges;

/*!
 @property   boundingBox
 @brief      The bounding box of a graph
 @details    Optional data containing the bounding box, set with
			 \path [use as bounding box] ....
 */
@property (assign) NSRect boundingBox;

/*!
 @property  hasBoundingBox
 @brief     Returns true if this graph has a bounding box.
 */
@property (readonly) BOOL hasBoundingBox;


/*!
 @brief      Computes graph bounds.
 @result     Graph bounds.
 */
- (NSRect)bounds;

/*!
 @brief      Returns the set of edges incident to the given node set.
 @param      nds a set of nodes.
 @result     A set of incident edges.
 */
- (NSSet*)incidentEdgesForNodes:(NSSet*)nds;

/*!
 @brief      Returns the set of in-edges for this node.
 @param      nd a node.
 @result     A set of edges.
*/
- (NSSet*)inEdgesForNode:(Node*)nd;

/*!
 @brief      Returns the set of out-edges for this node.
 @param      nd a node.
 @result     A set of edges.
*/
- (NSSet*)outEdgesForNode:(Node*)nd;

/*!
 @brief      Gives a copy of the full subgraph with the given nodes.
 @param      nds a set of nodes.
 @result     A subgraph.
 */
- (Graph*)copyOfSubgraphWithNodes:(NSSet*)nds;

/*!
 @brief      Gives a set of edge-arrays that partition all of the edges in the graph.
 @result     An NSet of NSArrays of edges.
 */
- (NSSet*)pathCover;

/*!
 @brief      Adds a node.
 @param      node
 @result     A <tt>GraphChange</tt> recording this action.
 */
- (GraphChange*)addNode:(Node*)node;

/*!
 @brief      Removes a node.
 @param      node
 @result     A <tt>GraphChange</tt> recording this action.
 */
- (GraphChange*)removeNode:(Node*)node;

/*!
 @brief      Removes a set of nodes.
 @param      nds a set of nodes
 @result     A <tt>GraphChange</tt> recording this action.
 */
- (GraphChange*)removeNodes:(NSSet *)nds;

/*!
 @brief      Adds an edge.
 @param      edge
 @result     A <tt>GraphChange</tt> recording this action.
 */
- (GraphChange*)addEdge:(Edge*)edge;

/*!
 @brief      Removed an edge.
 @param      edge
 @result     A <tt>GraphChange</tt> recording this action.
 */
- (GraphChange*)removeEdge:(Edge*)edge;

/*!
 @brief      Removes a set of edges.
 @param      es a set of edges.
 @result     A <tt>GraphChange</tt> recording this action.
 */
- (GraphChange*)removeEdges:(NSSet *)es;

/*!
 @brief      Convenience function, intializes an edge with the given
             source and target and adds it.
 @param      source the source of the edge.
 @param      target the target of the edge.
 @result     A <tt>GraphChange</tt> recording this action.
 */
- (GraphChange*)addEdgeFrom:(Node*)source to:(Node*)target;

/*!
 @brief      Transform every node in the graph to screen space.
 @param      t a transformer
 */
- (void)applyTransformer:(Transformer*)t;

/*!
 @brief      Shift nodes by a given distance.
 @param      ns a set of nodes.
 @param      p an x and y distance, given as an NSPoint.
 @result     A <tt>GraphChange</tt> recording this action.
 */
- (GraphChange*)shiftNodes:(NSSet*)ns byPoint:(NSPoint)p;

/*!
 @brief      Insert the given graph into this one. Used for copy
             and paste.
 @param      g a graph.
 @result     A <tt>GraphChange</tt> recording this action.
 */
- (GraphChange*)insertGraph:(Graph*)g;

/*!
 @brief      Flip the subgraph defined by the given node set
             horizontally.
 @param      nds a set of nodes.
 @result     A <tt>GraphChange</tt> recording this action.
 */
- (GraphChange*)flipHorizontalNodes:(NSSet*)nds;

/*!
 @brief      Flip the subgraph defined by the given node set
             vertically.
 @param      nds a set of nodes.
 @result     A <tt>GraphChange</tt> recording this action.
 */
- (GraphChange*)flipVerticalNodes:(NSSet*)nds;

/*!
 @brief      Apply a graph change.
 @details    An undo manager should maintain a stack of GraphChange
             objects returned. To undo a GraphChange, call this method
             with <tt>[change inverse]</tt> as is argument.
 @param      ch a graph change.
 */
- (void)applyGraphChange:(GraphChange*)ch;

/*!
 @brief      The TikZ representation of this graph.
 @details    The TikZ representation of this graph. The TikZ code should
             contain enough data to totally reconstruct the graph.
 @result     A string containing TikZ code.
 */
- (NSString*)tikz;

/*!
 @brief      Copy the node set and return a table of copies, whose
             keys are the original nodes. This is used to save the state
	         of a set of nodes in a GraphChange.
 @param      nds a set of nodes.
 @result     A <tt>BasicMapTable</tt> of node copies.
 */
+ (BasicMapTable*)nodeTableForNodes:(NSSet*)nds;

/*!
 @brief      Copy the edge set and return a table of copies, whose
             keys are the original edges. This is used to save the state
	         of a set of edges in a GraphChange.
 @param      es a set of edges.
 @result     A <tt>BasicMapTable</tt> of edge copies.
 */
+ (BasicMapTable*)edgeTableForEdges:(NSSet*)es;

/*!
 @brief      Compute the bounds for a set of nodes.
 @param      nds a set of nodes.
 @result     The bounds.
 */
+ (NSRect)boundsForNodeSet:(NSSet *)nds;

/*!
 @brief      Factory method for constructing graphs.
 @result     An empty graph.
 */
+ (Graph*)graph;

@end

// vi:ft=objc:noet:ts=4:sts=4:sw=4
