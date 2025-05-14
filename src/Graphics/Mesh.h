#pragma once

#include <cstdint>
#include <Utils/List.h>

#include "Math/Vec4.h"
#include "Renderer.h"

namespace g3d {
/**
 * Data structure to hold a mesh of points to be connected and rendered.
 */
class Mesh {
protected:
    /**
     * Inner data structure for each element of the mesh. Contains the world coordinate
     * of the point, its visibility, its color (based on z coordinate), and (if visible)
     * its calculated screen coordinate.
     */
    struct MeshElement {
        Vec4 world;
        bool visible;
        uint8_t color;
        Vec4 screen;
    };

    /** List that contains the rows of the mesh, where each row is a list of mesh elements. */
    bp::List<bp::List<MeshElement>> elements;

    /**
     * Updates the screen position of all the visible points in the Mesh.
     * 
     * @param renderer Reference to the `Renderer` object to use to calculate
     *                 screen coordinates.
     */
    void UpdateScreenLocations(const Renderer& renderer) {
        for (size_t i = 0; i < elements.Size(); ++i) {
            auto& row = *elements.Get(i);
            for (size_t j = 0; j < row.Size(); ++j) {
                MeshElement& el = *row.Get(j);
                if (el.visible) {
                    renderer.ConvertPoint(el.world, el.screen);
                }
            }
        }
    }

public:
    ~Mesh() {
        for (size_t i = 0; i < elements.Size(); ++i) {
            auto* row = elements.Get(i);
            while (!row->IsEmpty()) {
                delete row->RemoveLast();
            }
            delete row;
        }
    }

    /**
     * Creates another row for storing points in the Mesh. Calls to `AddPoint`
     * will add to the most recently created row.
     */
    void AddRow() {
        elements.Add(new bp::List<MeshElement>);
    }

    /**
     * Adds a 3D point in world space to the Mesh.
     * 
     * @param[in] x X coordinate of the point.
     * @param[in] y Y coordinate of the point.
     * @param[in] z Z coordinate of the point.
     * 
     * @return Error code corresponding to operation status: `ERR_MESH_FULL` if the
     *         mesh has already been filled or `ERR_SUCCESS` otherwise.
     */
    void AddPoint(double x, double y, double z) {
        MeshElement* newElement = new MeshElement;
        newElement->world.x = x;
        newElement->world.y = y;
        newElement->world.z = z;
        newElement->world.w = 1.0;
        elements.Get(elements.Size() - 1)->Add(newElement);
    }

    /**
     * Gets the number of rows in the Mesh.
     * 
     * @return Number of rows in the Mesh.
     */
    size_t GetNumRows() const {
        return elements.Size();
    }

    /**
     * Gets the number of elements in a given row.
     * 
     * @param[in] rowIdx Index of the row to get the number of elements in.
     * 
     * @return Number of elements in the specified row.
     */
    size_t GetNumElementsInRow(size_t rowIdx) const {
        if (rowIdx >= elements.Size()) {
            return 0;
        }
        return elements.Get(rowIdx)->Size();
    }

    /**
     * Gets the element at the specified location.
     * 
     * @param[in] row Index of the row.
     * @param[in] idx Index of the element within the row.
     * 
     * @return Pointer to the `MeshElement` at the specified location or
     *         `nullptr` if there is no element at that location.
     */
    const MeshElement* GetElement(size_t row, size_t idx) const {
        if (row >= elements.Size() || idx >= elements.Get(row)->Size()) {
            return nullptr;
        }
        return elements.Get(row)->Get(idx);
    }

    /**
     * Determines if there is a `MeshElement` in the specified location.
     * 
     * @param[in] row Index of the row.
     * @param[in] idx Index of the element within the row.
     * 
     * @return `true` if there is an element at the specified location or `false`
     *         otherwise.
     */
    bool ElementExists(size_t row, size_t idx) const {
        return row < elements.Size() && idx < elements.Get(row)->Size();
    }

    /**
     * Updates the visibility of all the points in the Mesh.
     * 
     * @param renderer Reference to the `Renderer` object to use to determine visibility.
     */
    void UpdateVisibility(const Renderer& renderer) {
        for (size_t i = 0; i < elements.Size(); ++i) {
            auto& row = *elements.Get(i);
            for (size_t j = 0; j < row.Size(); ++j) {
                MeshElement& el = *row.Get(j);
                el.visible = renderer.IsVisible(el.world);
            }
        }
    }

    /**
     * Updates the color (based on z coordinate) of all the visible points in the Mesh.
     * 
     * @param renderer Reference to the `Renderer` object to use to determine color.
     */
    void UpdateColor(const Renderer& renderer) {
        // Determine min and max attained z coordinates by the function
        const MeshElement* firstElement = GetElement(0, 0);
        if (firstElement == nullptr) {
            return; // no elements in the mesh--nothing to do
        }
        double minZAttained = firstElement->world.z;
        double maxZAttained = firstElement->world.z;
        for (size_t i = 0; i < elements.Size(); ++i) {
            auto& row = *elements.Get(i);
            for (size_t j = 0; j < row.Size(); ++j) {
                MeshElement& el = *row.Get(j);
                if (el.visible) {
                    if (el.world.z < minZAttained) {
                        minZAttained = el.world.z;
                    }
                    if (el.world.z > maxZAttained) {
                        maxZAttained = el.world.z;
                    }
                }
            }
        }
        for (size_t i = 0; i < elements.Size(); ++i) {
            auto& row = *elements.Get(i);
            for (size_t j = 0; j < row.Size(); ++j) {
                MeshElement& el = *row.Get(j);
                if (el.visible) {
                    el.color = renderer.GetColor(el.world.z, minZAttained, maxZAttained);
                }
            }
        }
    }

    /**
     * Draws the Mesh on the screen.
     */
    void Render(const Renderer& renderer) {
        UpdateScreenLocations(renderer);
        for (size_t i = 0; i < elements.Size(); ++i) {
            auto& row = *elements.Get(i);
            for (size_t j = 0; j < row.Size(); ++j) {
                const MeshElement& el = *row.Get(j);
                if (el.visible) {
                    gfx_SetColor(el.color);
                    if (ElementExists(i + 1, j)) {
                        const MeshElement& neighbor = *GetElement(i + 1, j);
                        if (neighbor.visible) {
                            gfx_Line(el.screen.x, el.screen.y, neighbor.screen.x, neighbor.screen.y);
                        }
                    }
                    if (ElementExists(i, j + 1)) {
                        const MeshElement& neighbor = *GetElement(i, j + 1);
                        if (neighbor.visible) {
                            gfx_Line(el.screen.x, el.screen.y, neighbor.screen.x, neighbor.screen.y);
                        }
                    }
                }
            }
        }
    }

private:
    /**
     * Helper function to render the connection between two elements of the Mesh.
     * 
     * @param[in] e1 First element to connect.
     * @param[in] e2 Second element to connect.
     */
    void ConnectElements(const MeshElement& e1, const MeshElement& e2) const {
        // TODO: Clipping code to generate a new point if one point is not visible
        if (e1.visible && e2.visible) {
            gfx_SetColor(e1.color);
            gfx_Line(e1.screen.x, e1.screen.y, e2.screen.x, e2.screen.y);
        }
    }
    
};

}