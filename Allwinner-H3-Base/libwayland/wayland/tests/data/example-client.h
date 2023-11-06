/* SCANNER TEST */

#ifndef WAYLAND_CLIENT_PROTOCOL_H
#define WAYLAND_CLIENT_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>
#include "wayland-client.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @page page_wayland The wayland protocol
 * @section page_ifaces_wayland Interfaces
 * - @subpage page_iface_wl_display - core global object
 * - @subpage page_iface_wl_registry - global registry object
 * - @subpage page_iface_wl_callback - callback object
 * - @subpage page_iface_wl_compositor - the compositor singleton
 * - @subpage page_iface_wl_shm_pool - a shared memory pool
 * - @subpage page_iface_wl_shm - shared memory support
 * - @subpage page_iface_wl_buffer - content for a wl_surface
 * - @subpage page_iface_wl_data_offer - offer to transfer data
 * - @subpage page_iface_wl_data_source - offer to transfer data
 * - @subpage page_iface_wl_data_device - data transfer device
 * - @subpage page_iface_wl_data_device_manager - data transfer interface
 * - @subpage page_iface_wl_shell - create desktop-style surfaces
 * - @subpage page_iface_wl_shell_surface - desktop-style metadata interface
 * - @subpage page_iface_wl_surface - an onscreen surface
 * - @subpage page_iface_wl_seat - group of input devices
 * - @subpage page_iface_wl_pointer - pointer input device
 * - @subpage page_iface_wl_keyboard - keyboard input device
 * - @subpage page_iface_wl_touch - touchscreen input device
 * - @subpage page_iface_wl_output - compositor output region
 * - @subpage page_iface_wl_region - region interface
 * - @subpage page_iface_wl_subcompositor - sub-surface compositing
 * - @subpage page_iface_wl_subsurface - sub-surface interface to a wl_surface
 * @section page_copyright_wayland Copyright
 * <pre>
 *
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2010-2011 Intel Corporation
 * Copyright © 2012-2013 Collabora, Ltd.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * </pre>
 */
struct wl_buffer;
struct wl_callback;
struct wl_compositor;
struct wl_data_device;
struct wl_data_device_manager;
struct wl_data_offer;
struct wl_data_source;
struct wl_display;
struct wl_keyboard;
struct wl_output;
struct wl_pointer;
struct wl_region;
struct wl_registry;
struct wl_seat;
struct wl_shell;
struct wl_shell_surface;
struct wl_shm;
struct wl_shm_pool;
struct wl_subcompositor;
struct wl_subsurface;
struct wl_surface;
struct wl_touch;

/**
 * @page page_iface_wl_display wl_display
 * @section page_iface_wl_display_desc Description
 *
 * The core global object.  This is a special singleton object.  It
 * is used for internal Wayland protocol features.
 * @section page_iface_wl_display_api API
 * See @ref iface_wl_display.
 */
/**
 * @defgroup iface_wl_display The wl_display interface
 *
 * The core global object.  This is a special singleton object.  It
 * is used for internal Wayland protocol features.
 */
extern const struct wl_interface wl_display_interface;
/**
 * @page page_iface_wl_registry wl_registry
 * @section page_iface_wl_registry_desc Description
 *
 * The singleton global registry object.  The server has a number of
 * global objects that are available to all clients.  These objects
 * typically represent an actual object in the server (for example,
 * an input device) or they are singleton objects that provide
 * extension functionality.
 *
 * When a client creates a registry object, the registry object
 * will emit a global event for each global currently in the
 * registry.  Globals come and go as a result of device or
 * monitor hotplugs, reconfiguration or other events, and the
 * registry will send out global and global_remove events to
 * keep the client up to date with the changes.  To mark the end
 * of the initial burst of events, the client can use the
 * wl_display.sync request immediately after calling
 * wl_display.get_registry.
 *
 * A client can bind to a global object by using the bind
 * request.  This creates a client-side handle that lets the object
 * emit events to the client and lets the client invoke requests on
 * the object.
 * @section page_iface_wl_registry_api API
 * See @ref iface_wl_registry.
 */
/**
 * @defgroup iface_wl_registry The wl_registry interface
 *
 * The singleton global registry object.  The server has a number of
 * global objects that are available to all clients.  These objects
 * typically represent an actual object in the server (for example,
 * an input device) or they are singleton objects that provide
 * extension functionality.
 *
 * When a client creates a registry object, the registry object
 * will emit a global event for each global currently in the
 * registry.  Globals come and go as a result of device or
 * monitor hotplugs, reconfiguration or other events, and the
 * registry will send out global and global_remove events to
 * keep the client up to date with the changes.  To mark the end
 * of the initial burst of events, the client can use the
 * wl_display.sync request immediately after calling
 * wl_display.get_registry.
 *
 * A client can bind to a global object by using the bind
 * request.  This creates a client-side handle that lets the object
 * emit events to the client and lets the client invoke requests on
 * the object.
 */
extern const struct wl_interface wl_registry_interface;
/**
 * @page page_iface_wl_callback wl_callback
 * @section page_iface_wl_callback_desc Description
 *
 * Clients can handle the 'done' event to get notified when
 * the related request is done.
 * @section page_iface_wl_callback_api API
 * See @ref iface_wl_callback.
 */
/**
 * @defgroup iface_wl_callback The wl_callback interface
 *
 * Clients can handle the 'done' event to get notified when
 * the related request is done.
 */
extern const struct wl_interface wl_callback_interface;
/**
 * @page page_iface_wl_compositor wl_compositor
 * @section page_iface_wl_compositor_desc Description
 *
 * A compositor.  This object is a singleton global.  The
 * compositor is in charge of combining the contents of multiple
 * surfaces into one displayable output.
 * @section page_iface_wl_compositor_api API
 * See @ref iface_wl_compositor.
 */
/**
 * @defgroup iface_wl_compositor The wl_compositor interface
 *
 * A compositor.  This object is a singleton global.  The
 * compositor is in charge of combining the contents of multiple
 * surfaces into one displayable output.
 */
extern const struct wl_interface wl_compositor_interface;
/**
 * @page page_iface_wl_shm_pool wl_shm_pool
 * @section page_iface_wl_shm_pool_desc Description
 *
 * The wl_shm_pool object encapsulates a piece of memory shared
 * between the compositor and client.  Through the wl_shm_pool
 * object, the client can allocate shared memory wl_buffer objects.
 * All objects created through the same pool share the same
 * underlying mapped memory. Reusing the mapped memory avoids the
 * setup/teardown overhead and is useful when interactively resizing
 * a surface or for many small buffers.
 * @section page_iface_wl_shm_pool_api API
 * See @ref iface_wl_shm_pool.
 */
/**
 * @defgroup iface_wl_shm_pool The wl_shm_pool interface
 *
 * The wl_shm_pool object encapsulates a piece of memory shared
 * between the compositor and client.  Through the wl_shm_pool
 * object, the client can allocate shared memory wl_buffer objects.
 * All objects created through the same pool share the same
 * underlying mapped memory. Reusing the mapped memory avoids the
 * setup/teardown overhead and is useful when interactively resizing
 * a surface or for many small buffers.
 */
extern const struct wl_interface wl_shm_pool_interface;
/**
 * @page page_iface_wl_shm wl_shm
 * @section page_iface_wl_shm_desc Description
 *
 * A singleton global object that provides support for shared
 * memory.
 *
 * Clients can create wl_shm_pool objects using the create_pool
 * request.
 *
 * At connection setup time, the wl_shm object emits one or more
 * format events to inform clients about the valid pixel formats
 * that can be used for buffers.
 * @section page_iface_wl_shm_api API
 * See @ref iface_wl_shm.
 */
/**
 * @defgroup iface_wl_shm The wl_shm interface
 *
 * A singleton global object that provides support for shared
 * memory.
 *
 * Clients can create wl_shm_pool objects using the create_pool
 * request.
 *
 * At connection setup time, the wl_shm object emits one or more
 * format events to inform clients about the valid pixel formats
 * that can be used for buffers.
 */
extern const struct wl_interface wl_shm_interface;
/**
 * @page page_iface_wl_buffer wl_buffer
 * @section page_iface_wl_buffer_desc Description
 *
 * A buffer provides the content for a wl_surface. Buffers are
 * created through factory interfaces such as wl_drm, wl_shm or
 * similar. It has a width and a height and can be attached to a
 * wl_surface, but the mechanism by which a client provides and
 * updates the contents is defined by the buffer factory interface.
 * @section page_iface_wl_buffer_api API
 * See @ref iface_wl_buffer.
 */
/**
 * @defgroup iface_wl_buffer The wl_buffer interface
 *
 * A buffer provides the content for a wl_surface. Buffers are
 * created through factory interfaces such as wl_drm, wl_shm or
 * similar. It has a width and a height and can be attached to a
 * wl_surface, but the mechanism by which a client provides and
 * updates the contents is defined by the buffer factory interface.
 */
extern const struct wl_interface wl_buffer_interface;
/**
 * @page page_iface_wl_data_offer wl_data_offer
 * @section page_iface_wl_data_offer_desc Description
 *
 * A wl_data_offer represents a piece of data offered for transfer
 * by another client (the source client).  It is used by the
 * copy-and-paste and drag-and-drop mechanisms.  The offer
 * describes the different mime types that the data can be
 * converted to and provides the mechanism for transferring the
 * data directly from the source client.
 * @section page_iface_wl_data_offer_api API
 * See @ref iface_wl_data_offer.
 */
/**
 * @defgroup iface_wl_data_offer The wl_data_offer interface
 *
 * A wl_data_offer represents a piece of data offered for transfer
 * by another client (the source client).  It is used by the
 * copy-and-paste and drag-and-drop mechanisms.  The offer
 * describes the different mime types that the data can be
 * converted to and provides the mechanism for transferring the
 * data directly from the source client.
 */
extern const struct wl_interface wl_data_offer_interface;
/**
 * @page page_iface_wl_data_source wl_data_source
 * @section page_iface_wl_data_source_desc Description
 *
 * The wl_data_source object is the source side of a wl_data_offer.
 * It is created by the source client in a data transfer and
 * provides a way to describe the offered data and a way to respond
 * to requests to transfer the data.
 * @section page_iface_wl_data_source_api API
 * See @ref iface_wl_data_source.
 */
/**
 * @defgroup iface_wl_data_source The wl_data_source interface
 *
 * The wl_data_source object is the source side of a wl_data_offer.
 * It is created by the source client in a data transfer and
 * provides a way to describe the offered data and a way to respond
 * to requests to transfer the data.
 */
extern const struct wl_interface wl_data_source_interface;
/**
 * @page page_iface_wl_data_device wl_data_device
 * @section page_iface_wl_data_device_desc Description
 *
 * There is one wl_data_device per seat which can be obtained
 * from the global wl_data_device_manager singleton.
 *
 * A wl_data_device provides access to inter-client data transfer
 * mechanisms such as copy-and-paste and drag-and-drop.
 * @section page_iface_wl_data_device_api API
 * See @ref iface_wl_data_device.
 */
/**
 * @defgroup iface_wl_data_device The wl_data_device interface
 *
 * There is one wl_data_device per seat which can be obtained
 * from the global wl_data_device_manager singleton.
 *
 * A wl_data_device provides access to inter-client data transfer
 * mechanisms such as copy-and-paste and drag-and-drop.
 */
extern const struct wl_interface wl_data_device_interface;
/**
 * @page page_iface_wl_data_device_manager wl_data_device_manager
 * @section page_iface_wl_data_device_manager_desc Description
 *
 * The wl_data_device_manager is a singleton global object that
 * provides access to inter-client data transfer mechanisms such as
 * copy-and-paste and drag-and-drop.  These mechanisms are tied to
 * a wl_seat and this interface lets a client get a wl_data_device
 * corresponding to a wl_seat.
 *
 * Depending on the version bound, the objects created from the bound
 * wl_data_device_manager object will have different requirements for
 * functioning properly. See wl_data_source.set_actions,
 * wl_data_offer.accept and wl_data_offer.finish for details.
 * @section page_iface_wl_data_device_manager_api API
 * See @ref iface_wl_data_device_manager.
 */
/**
 * @defgroup iface_wl_data_device_manager The wl_data_device_manager interface
 *
 * The wl_data_device_manager is a singleton global object that
 * provides access to inter-client data transfer mechanisms such as
 * copy-and-paste and drag-and-drop.  These mechanisms are tied to
 * a wl_seat and this interface lets a client get a wl_data_device
 * corresponding to a wl_seat.
 *
 * Depending on the version bound, the objects created from the bound
 * wl_data_device_manager object will have different requirements for
 * functioning properly. See wl_data_source.set_actions,
 * wl_data_offer.accept and wl_data_offer.finish for details.
 */
extern const struct wl_interface wl_data_device_manager_interface;
/**
 * @page page_iface_wl_shell wl_shell
 * @section page_iface_wl_shell_desc Description
 *
 * This interface is implemented by servers that provide
 * desktop-style user interfaces.
 *
 * It allows clients to associate a wl_shell_surface with
 * a basic surface.
 * @section page_iface_wl_shell_api API
 * See @ref iface_wl_shell.
 */
/**
 * @defgroup iface_wl_shell The wl_shell interface
 *
 * This interface is implemented by servers that provide
 * desktop-style user interfaces.
 *
 * It allows clients to associate a wl_shell_surface with
 * a basic surface.
 */
extern const struct wl_interface wl_shell_interface;
/**
 * @page page_iface_wl_shell_surface wl_shell_surface
 * @section page_iface_wl_shell_surface_desc Description
 *
 * An interface that may be implemented by a wl_surface, for
 * implementations that provide a desktop-style user interface.
 *
 * It provides requests to treat surfaces like toplevel, fullscreen
 * or popup windows, move, resize or maximize them, associate
 * metadata like title and class, etc.
 *
 * On the server side the object is automatically destroyed when
 * the related wl_surface is destroyed. On the client side,
 * wl_shell_surface_destroy() must be called before destroying
 * the wl_surface object.
 * @section page_iface_wl_shell_surface_api API
 * See @ref iface_wl_shell_surface.
 */
/**
 * @defgroup iface_wl_shell_surface The wl_shell_surface interface
 *
 * An interface that may be implemented by a wl_surface, for
 * implementations that provide a desktop-style user interface.
 *
 * It provides requests to treat surfaces like toplevel, fullscreen
 * or popup windows, move, resize or maximize them, associate
 * metadata like title and class, etc.
 *
 * On the server side the object is automatically destroyed when
 * the related wl_surface is destroyed. On the client side,
 * wl_shell_surface_destroy() must be called before destroying
 * the wl_surface object.
 */
extern const struct wl_interface wl_shell_surface_interface;
/**
 * @page page_iface_wl_surface wl_surface
 * @section page_iface_wl_surface_desc Description
 *
 * A surface is a rectangular area that is displayed on the screen.
 * It has a location, size and pixel contents.
 *
 * The size of a surface (and relative positions on it) is described
 * in surface-local coordinates, which may differ from the buffer
 * coordinates of the pixel content, in case a buffer_transform
 * or a buffer_scale is used.
 *
 * A surface without a "role" is fairly useless: a compositor does
 * not know where, when or how to present it. The role is the
 * purpose of a wl_surface. Examples of roles are a cursor for a
 * pointer (as set by wl_pointer.set_cursor), a drag icon
 * (wl_data_device.start_drag), a sub-surface
 * (wl_subcompositor.get_subsurface), and a window as defined by a
 * shell protocol (e.g. wl_shell.get_shell_surface).
 *
 * A surface can have only one role at a time. Initially a
 * wl_surface does not have a role. Once a wl_surface is given a
 * role, it is set permanently for the whole lifetime of the
 * wl_surface object. Giving the current role again is allowed,
 * unless explicitly forbidden by the relevant interface
 * specification.
 *
 * Surface roles are given by requests in other interfaces such as
 * wl_pointer.set_cursor. The request should explicitly mention
 * that this request gives a role to a wl_surface. Often, this
 * request also creates a new protocol object that represents the
 * role and adds additional functionality to wl_surface. When a
 * client wants to destroy a wl_surface, they must destroy this 'role
 * object' before the wl_surface.
 *
 * Destroying the role object does not remove the role from the
 * wl_surface, but it may stop the wl_surface from "playing the role".
 * For instance, if a wl_subsurface object is destroyed, the wl_surface
 * it was created for will be unmapped and forget its position and
 * z-order. It is allowed to create a wl_subsurface for the same
 * wl_surface again, but it is not allowed to use the wl_surface as
 * a cursor (cursor is a different role than sub-surface, and role
 * switching is not allowed).
 * @section page_iface_wl_surface_api API
 * See @ref iface_wl_surface.
 */
/**
 * @defgroup iface_wl_surface The wl_surface interface
 *
 * A surface is a rectangular area that is displayed on the screen.
 * It has a location, size and pixel contents.
 *
 * The size of a surface (and relative positions on it) is described
 * in surface-local coordinates, which may differ from the buffer
 * coordinates of the pixel content, in case a buffer_transform
 * or a buffer_scale is used.
 *
 * A surface without a "role" is fairly useless: a compositor does
 * not know where, when or how to present it. The role is the
 * purpose of a wl_surface. Examples of roles are a cursor for a
 * pointer (as set by wl_pointer.set_cursor), a drag icon
 * (wl_data_device.start_drag), a sub-surface
 * (wl_subcompositor.get_subsurface), and a window as defined by a
 * shell protocol (e.g. wl_shell.get_shell_surface).
 *
 * A surface can have only one role at a time. Initially a
 * wl_surface does not have a role. Once a wl_surface is given a
 * role, it is set permanently for the whole lifetime of the
 * wl_surface object. Giving the current role again is allowed,
 * unless explicitly forbidden by the relevant interface
 * specification.
 *
 * Surface roles are given by requests in other interfaces such as
 * wl_pointer.set_cursor. The request should explicitly mention
 * that this request gives a role to a wl_surface. Often, this
 * request also creates a new protocol object that represents the
 * role and adds additional functionality to wl_surface. When a
 * client wants to destroy a wl_surface, they must destroy this 'role
 * object' before the wl_surface.
 *
 * Destroying the role object does not remove the role from the
 * wl_surface, but it may stop the wl_surface from "playing the role".
 * For instance, if a wl_subsurface object is destroyed, the wl_surface
 * it was created for will be unmapped and forget its position and
 * z-order. It is allowed to create a wl_subsurface for the same
 * wl_surface again, but it is not allowed to use the wl_surface as
 * a cursor (cursor is a different role than sub-surface, and role
 * switching is not allowed).
 */
extern const struct wl_interface wl_surface_interface;
/**
 * @page page_iface_wl_seat wl_seat
 * @section page_iface_wl_seat_desc Description
 *
 * A seat is a group of keyboards, pointer and touch devices. This
 * object is published as a global during start up, or when such a
 * device is hot plugged.  A seat typically has a pointer and
 * maintains a keyboard focus and a pointer focus.
 * @section page_iface_wl_seat_api API
 * See @ref iface_wl_seat.
 */
/**
 * @defgroup iface_wl_seat The wl_seat interface
 *
 * A seat is a group of keyboards, pointer and touch devices. This
 * object is published as a global during start up, or when such a
 * device is hot plugged.  A seat typically has a pointer and
 * maintains a keyboard focus and a pointer focus.
 */
extern const struct wl_interface wl_seat_interface;
/**
 * @page page_iface_wl_pointer wl_pointer
 * @section page_iface_wl_pointer_desc Description
 *
 * The wl_pointer interface represents one or more input devices,
 * such as mice, which control the pointer location and pointer_focus
 * of a seat.
 *
 * The wl_pointer interface generates motion, enter and leave
 * events for the surfaces that the pointer is located over,
 * and button and axis events for button presses, button releases
 * and scrolling.
 * @section page_iface_wl_pointer_api API
 * See @ref iface_wl_pointer.
 */
/**
 * @defgroup iface_wl_pointer The wl_pointer interface
 *
 * The wl_pointer interface represents one or more input devices,
 * such as mice, which control the pointer location and pointer_focus
 * of a seat.
 *
 * The wl_pointer interface generates motion, enter and leave
 * events for the surfaces that the pointer is located over,
 * and button and axis events for button presses, button releases
 * and scrolling.
 */
extern const struct wl_interface wl_pointer_interface;
/**
 * @page page_iface_wl_keyboard wl_keyboard
 * @section page_iface_wl_keyboard_desc Description
 *
 * The wl_keyboard interface represents one or more keyboards
 * associated with a seat.
 * @section page_iface_wl_keyboard_api API
 * See @ref iface_wl_keyboard.
 */
/**
 * @defgroup iface_wl_keyboard The wl_keyboard interface
 *
 * The wl_keyboard interface represents one or more keyboards
 * associated with a seat.
 */
extern const struct wl_interface wl_keyboard_interface;
/**
 * @page page_iface_wl_touch wl_touch
 * @section page_iface_wl_touch_desc Description
 *
 * The wl_touch interface represents a touchscreen
 * associated with a seat.
 *
 * Touch interactions can consist of one or more contacts.
 * For each contact, a series of events is generated, starting
 * with a down event, followed by zero or more motion events,
 * and ending with an up event. Events relating to the same
 * contact point can be identified by the ID of the sequence.
 * @section page_iface_wl_touch_api API
 * See @ref iface_wl_touch.
 */
/**
 * @defgroup iface_wl_touch The wl_touch interface
 *
 * The wl_touch interface represents a touchscreen
 * associated with a seat.
 *
 * Touch interactions can consist of one or more contacts.
 * For each contact, a series of events is generated, starting
 * with a down event, followed by zero or more motion events,
 * and ending with an up event. Events relating to the same
 * contact point can be identified by the ID of the sequence.
 */
extern const struct wl_interface wl_touch_interface;
/**
 * @page page_iface_wl_output wl_output
 * @section page_iface_wl_output_desc Description
 *
 * An output describes part of the compositor geometry.  The
 * compositor works in the 'compositor coordinate system' and an
 * output corresponds to a rectangular area in that space that is
 * actually visible.  This typically corresponds to a monitor that
 * displays part of the compositor space.  This object is published
 * as global during start up, or when a monitor is hotplugged.
 * @section page_iface_wl_output_api API
 * See @ref iface_wl_output.
 */
/**
 * @defgroup iface_wl_output The wl_output interface
 *
 * An output describes part of the compositor geometry.  The
 * compositor works in the 'compositor coordinate system' and an
 * output corresponds to a rectangular area in that space that is
 * actually visible.  This typically corresponds to a monitor that
 * displays part of the compositor space.  This object is published
 * as global during start up, or when a monitor is hotplugged.
 */
extern const struct wl_interface wl_output_interface;
/**
 * @page page_iface_wl_region wl_region
 * @section page_iface_wl_region_desc Description
 *
 * A region object describes an area.
 *
 * Region objects are used to describe the opaque and input
 * regions of a surface.
 * @section page_iface_wl_region_api API
 * See @ref iface_wl_region.
 */
/**
 * @defgroup iface_wl_region The wl_region interface
 *
 * A region object describes an area.
 *
 * Region objects are used to describe the opaque and input
 * regions of a surface.
 */
extern const struct wl_interface wl_region_interface;
/**
 * @page page_iface_wl_subcompositor wl_subcompositor
 * @section page_iface_wl_subcompositor_desc Description
 *
 * The global interface exposing sub-surface compositing capabilities.
 * A wl_surface, that has sub-surfaces associated, is called the
 * parent surface. Sub-surfaces can be arbitrarily nested and create
 * a tree of sub-surfaces.
 *
 * The root surface in a tree of sub-surfaces is the main
 * surface. The main surface cannot be a sub-surface, because
 * sub-surfaces must always have a parent.
 *
 * A main surface with its sub-surfaces forms a (compound) window.
 * For window management purposes, this set of wl_surface objects is
 * to be considered as a single window, and it should also behave as
 * such.
 *
 * The aim of sub-surfaces is to offload some of the compositing work
 * within a window from clients to the compositor. A prime example is
 * a video player with decorations and video in separate wl_surface
 * objects. This should allow the compositor to pass YUV video buffer
 * processing to dedicated overlay hardware when possible.
 * @section page_iface_wl_subcompositor_api API
 * See @ref iface_wl_subcompositor.
 */
/**
 * @defgroup iface_wl_subcompositor The wl_subcompositor interface
 *
 * The global interface exposing sub-surface compositing capabilities.
 * A wl_surface, that has sub-surfaces associated, is called the
 * parent surface. Sub-surfaces can be arbitrarily nested and create
 * a tree of sub-surfaces.
 *
 * The root surface in a tree of sub-surfaces is the main
 * surface. The main surface cannot be a sub-surface, because
 * sub-surfaces must always have a parent.
 *
 * A main surface with its sub-surfaces forms a (compound) window.
 * For window management purposes, this set of wl_surface objects is
 * to be considered as a single window, and it should also behave as
 * such.
 *
 * The aim of sub-surfaces is to offload some of the compositing work
 * within a window from clients to the compositor. A prime example is
 * a video player with decorations and video in separate wl_surface
 * objects. This should allow the compositor to pass YUV video buffer
 * processing to dedicated overlay hardware when possible.
 */
extern const struct wl_interface wl_subcompositor_interface;
/**
 * @page page_iface_wl_subsurface wl_subsurface
 * @section page_iface_wl_subsurface_desc Description
 *
 * An additional interface to a wl_surface object, which has been
 * made a sub-surface. A sub-surface has one parent surface. A
 * sub-surface's size and position are not limited to that of the parent.
 * Particularly, a sub-surface is not automatically clipped to its
 * parent's area.
 *
 * A sub-surface becomes mapped, when a non-NULL wl_buffer is applied
 * and the parent surface is mapped. The order of which one happens
 * first is irrelevant. A sub-surface is hidden if the parent becomes
 * hidden, or if a NULL wl_buffer is applied. These rules apply
 * recursively through the tree of surfaces.
 *
 * The behaviour of a wl_surface.commit request on a sub-surface
 * depends on the sub-surface's mode. The possible modes are
 * synchronized and desynchronized, see methods
 * wl_subsurface.set_sync and wl_subsurface.set_desync. Synchronized
 * mode caches the wl_surface state to be applied when the parent's
 * state gets applied, and desynchronized mode applies the pending
 * wl_surface state directly. A sub-surface is initially in the
 * synchronized mode.
 *
 * Sub-surfaces have also other kind of state, which is managed by
 * wl_subsurface requests, as opposed to wl_surface requests. This
 * state includes the sub-surface position relative to the parent
 * surface (wl_subsurface.set_position), and the stacking order of
 * the parent and its sub-surfaces (wl_subsurface.place_above and
 * .place_below). This state is applied when the parent surface's
 * wl_surface state is applied, regardless of the sub-surface's mode.
 * As the exception, set_sync and set_desync are effective immediately.
 *
 * The main surface can be thought to be always in desynchronized mode,
 * since it does not have a parent in the sub-surfaces sense.
 *
 * Even if a sub-surface is in desynchronized mode, it will behave as
 * in synchronized mode, if its parent surface behaves as in
 * synchronized mode. This rule is applied recursively throughout the
 * tree of surfaces. This means, that one can set a sub-surface into
 * synchronized mode, and then assume that all its child and grand-child
 * sub-surfaces are synchronized, too, without explicitly setting them.
 *
 * If the wl_surface associated with the wl_subsurface is destroyed, the
 * wl_subsurface object becomes inert. Note, that destroying either object
 * takes effect immediately. If you need to synchronize the removal
 * of a sub-surface to the parent surface update, unmap the sub-surface
 * first by attaching a NULL wl_buffer, update parent, and then destroy
 * the sub-surface.
 *
 * If the parent wl_surface object is destroyed, the sub-surface is
 * unmapped.
 * @section page_iface_wl_subsurface_api API
 * See @ref iface_wl_subsurface.
 */
/**
 * @defgroup iface_wl_subsurface The wl_subsurface interface
 *
 * An additional interface to a wl_surface object, which has been
 * made a sub-surface. A sub-surface has one parent surface. A
 * sub-surface's size and position are not limited to that of the parent.
 * Particularly, a sub-surface is not automatically clipped to its
 * parent's area.
 *
 * A sub-surface becomes mapped, when a non-NULL wl_buffer is applied
 * and the parent surface is mapped. The order of which one happens
 * first is irrelevant. A sub-surface is hidden if the parent becomes
 * hidden, or if a NULL wl_buffer is applied. These rules apply
 * recursively through the tree of surfaces.
 *
 * The behaviour of a wl_surface.commit request on a sub-surface
 * depends on the sub-surface's mode. The possible modes are
 * synchronized and desynchronized, see methods
 * wl_subsurface.set_sync and wl_subsurface.set_desync. Synchronized
 * mode caches the wl_surface state to be applied when the parent's
 * state gets applied, and desynchronized mode applies the pending
 * wl_surface state directly. A sub-surface is initially in the
 * synchronized mode.
 *
 * Sub-surfaces have also other kind of state, which is managed by
 * wl_subsurface requests, as opposed to wl_surface requests. This
 * state includes the sub-surface position relative to the parent
 * surface (wl_subsurface.set_position), and the stacking order of
 * the parent and its sub-surfaces (wl_subsurface.place_above and
 * .place_below). This state is applied when the parent surface's
 * wl_surface state is applied, regardless of the sub-surface's mode.
 * As the exception, set_sync and set_desync are effective immediately.
 *
 * The main surface can be thought to be always in desynchronized mode,
 * since it does not have a parent in the sub-surfaces sense.
 *
 * Even if a sub-surface is in desynchronized mode, it will behave as
 * in synchronized mode, if its parent surface behaves as in
 * synchronized mode. This rule is applied recursively throughout the
 * tree of surfaces. This means, that one can set a sub-surface into
 * synchronized mode, and then assume that all its child and grand-child
 * sub-surfaces are synchronized, too, without explicitly setting them.
 *
 * If the wl_surface associated with the wl_subsurface is destroyed, the
 * wl_subsurface object becomes inert. Note, that destroying either object
 * takes effect immediately. If you need to synchronize the removal
 * of a sub-surface to the parent surface update, unmap the sub-surface
 * first by attaching a NULL wl_buffer, update parent, and then destroy
 * the sub-surface.
 *
 * If the parent wl_surface object is destroyed, the sub-surface is
 * unmapped.
 */
extern const struct wl_interface wl_subsurface_interface;

#ifndef WL_DISPLAY_ERROR_ENUM
#define WL_DISPLAY_ERROR_ENUM
/**
 * @ingroup iface_wl_display
 * global error values
 *
 * These errors are global and can be emitted in response to any
 * server request.
 */
enum wl_display_error {
	/**
	 * server couldn't find object
	 */
	WL_DISPLAY_ERROR_INVALID_OBJECT = 0,
	/**
	 * method doesn't exist on the specified interface
	 */
	WL_DISPLAY_ERROR_INVALID_METHOD = 1,
	/**
	 * server is out of memory
	 */
	WL_DISPLAY_ERROR_NO_MEMORY = 2,
};
#endif /* WL_DISPLAY_ERROR_ENUM */

/**
 * @ingroup iface_wl_display
 * @struct wl_display_listener
 */
struct wl_display_listener {
	/**
	 * fatal error event
	 *
	 * The error event is sent out when a fatal (non-recoverable)
	 * error has occurred. The object_id argument is the object where
	 * the error occurred, most often in response to a request to that
	 * object. The code identifies the error and is defined by the
	 * object interface. As such, each interface defines its own set of
	 * error codes. The message is a brief description of the error,
	 * for (debugging) convenience.
	 * @param object_id object where the error occurred
	 * @param code error code
	 * @param message error description
	 */
	void (*error)(void *data,
		      struct wl_display *wl_display,
		      void *object_id,
		      uint32_t code,
		      const char *message);
	/**
	 * acknowledge object ID deletion
	 *
	 * This event is used internally by the object ID management
	 * logic. When a client deletes an object, the server will send
	 * this event to acknowledge that it has seen the delete request.
	 * When the client receives this event, it will know that it can
	 * safely reuse the object ID.
	 * @param id deleted object ID
	 */
	void (*delete_id)(void *data,
			  struct wl_display *wl_display,
			  uint32_t id);
};

/**
 * @ingroup iface_wl_display
 */
static inline int
wl_display_add_listener(struct wl_display *wl_display,
			const struct wl_display_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_display,
				     (void (**)(void)) listener, data);
}

#define WL_DISPLAY_SYNC 0
#define WL_DISPLAY_GET_REGISTRY 1

/**
 * @ingroup iface_wl_display
 */
#define WL_DISPLAY_ERROR_SINCE_VERSION 1
/**
 * @ingroup iface_wl_display
 */
#define WL_DISPLAY_DELETE_ID_SINCE_VERSION 1

/**
 * @ingroup iface_wl_display
 */
#define WL_DISPLAY_SYNC_SINCE_VERSION 1
/**
 * @ingroup iface_wl_display
 */
#define WL_DISPLAY_GET_REGISTRY_SINCE_VERSION 1

/** @ingroup iface_wl_display */
static inline void
wl_display_set_user_data(struct wl_display *wl_display, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_display, user_data);
}

/** @ingroup iface_wl_display */
static inline void *
wl_display_get_user_data(struct wl_display *wl_display)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_display);
}

static inline uint32_t
wl_display_get_version(struct wl_display *wl_display)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_display);
}

/**
 * @ingroup iface_wl_display
 *
 * The sync request asks the server to emit the 'done' event
 * on the returned wl_callback object.  Since requests are
 * handled in-order and events are delivered in-order, this can
 * be used as a barrier to ensure all previous requests and the
 * resulting events have been handled.
 *
 * The object returned by this request will be destroyed by the
 * compositor after the callback is fired and as such the client must not
 * attempt to use it after that point.
 *
 * The callback_data passed in the callback is the event serial.
 */
static inline struct wl_callback *
wl_display_sync(struct wl_display *wl_display)
{
	struct wl_proxy *callback;

	callback = wl_proxy_marshal_constructor((struct wl_proxy *) wl_display,
			 WL_DISPLAY_SYNC, &wl_callback_interface, NULL);

	return (struct wl_callback *) callback;
}

/**
 * @ingroup iface_wl_display
 *
 * This request creates a registry object that allows the client
 * to list and bind the global objects available from the
 * compositor.
 */
static inline struct wl_registry *
wl_display_get_registry(struct wl_display *wl_display)
{
	struct wl_proxy *registry;

	registry = wl_proxy_marshal_constructor((struct wl_proxy *) wl_display,
			 WL_DISPLAY_GET_REGISTRY, &wl_registry_interface, NULL);

	return (struct wl_registry *) registry;
}

/**
 * @ingroup iface_wl_registry
 * @struct wl_registry_listener
 */
struct wl_registry_listener {
	/**
	 * announce global object
	 *
	 * Notify the client of global objects.
	 *
	 * The event notifies the client that a global object with the
	 * given name is now available, and it implements the given version
	 * of the given interface.
	 * @param name numeric name of the global object
	 * @param interface interface implemented by the object
	 * @param version interface version
	 */
	void (*global)(void *data,
		       struct wl_registry *wl_registry,
		       uint32_t name,
		       const char *interface,
		       uint32_t version);
	/**
	 * announce removal of global object
	 *
	 * Notify the client of removed global objects.
	 *
	 * This event notifies the client that the global identified by
	 * name is no longer available. If the client bound to the global
	 * using the bind request, the client should now destroy that
	 * object.
	 *
	 * The object remains valid and requests to the object will be
	 * ignored until the client destroys it, to avoid races between the
	 * global going away and a client sending a request to it.
	 * @param name numeric name of the global object
	 */
	void (*global_remove)(void *data,
			      struct wl_registry *wl_registry,
			      uint32_t name);
};

/**
 * @ingroup iface_wl_registry
 */
static inline int
wl_registry_add_listener(struct wl_registry *wl_registry,
			 const struct wl_registry_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_registry,
				     (void (**)(void)) listener, data);
}

#define WL_REGISTRY_BIND 0

/**
 * @ingroup iface_wl_registry
 */
#define WL_REGISTRY_GLOBAL_SINCE_VERSION 1
/**
 * @ingroup iface_wl_registry
 */
#define WL_REGISTRY_GLOBAL_REMOVE_SINCE_VERSION 1

/**
 * @ingroup iface_wl_registry
 */
#define WL_REGISTRY_BIND_SINCE_VERSION 1

/** @ingroup iface_wl_registry */
static inline void
wl_registry_set_user_data(struct wl_registry *wl_registry, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_registry, user_data);
}

/** @ingroup iface_wl_registry */
static inline void *
wl_registry_get_user_data(struct wl_registry *wl_registry)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_registry);
}

static inline uint32_t
wl_registry_get_version(struct wl_registry *wl_registry)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_registry);
}

/** @ingroup iface_wl_registry */
static inline void
wl_registry_destroy(struct wl_registry *wl_registry)
{
	wl_proxy_destroy((struct wl_proxy *) wl_registry);
}

/**
 * @ingroup iface_wl_registry
 *
 * Binds a new, client-created object to the server using the
 * specified name as the identifier.
 */
static inline void *
wl_registry_bind(struct wl_registry *wl_registry, uint32_t name, const struct wl_interface *interface, uint32_t version)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor_versioned((struct wl_proxy *) wl_registry,
			 WL_REGISTRY_BIND, interface, version, name, interface->name, version, NULL);

	return (void *) id;
}

/**
 * @ingroup iface_wl_callback
 * @struct wl_callback_listener
 */
struct wl_callback_listener {
	/**
	 * done event
	 *
	 * Notify the client when the related request is done.
	 * @param callback_data request-specific data for the callback
	 */
	void (*done)(void *data,
		     struct wl_callback *wl_callback,
		     uint32_t callback_data);
};

/**
 * @ingroup iface_wl_callback
 */
static inline int
wl_callback_add_listener(struct wl_callback *wl_callback,
			 const struct wl_callback_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_callback,
				     (void (**)(void)) listener, data);
}

/**
 * @ingroup iface_wl_callback
 */
#define WL_CALLBACK_DONE_SINCE_VERSION 1


/** @ingroup iface_wl_callback */
static inline void
wl_callback_set_user_data(struct wl_callback *wl_callback, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_callback, user_data);
}

/** @ingroup iface_wl_callback */
static inline void *
wl_callback_get_user_data(struct wl_callback *wl_callback)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_callback);
}

static inline uint32_t
wl_callback_get_version(struct wl_callback *wl_callback)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_callback);
}

/** @ingroup iface_wl_callback */
static inline void
wl_callback_destroy(struct wl_callback *wl_callback)
{
	wl_proxy_destroy((struct wl_proxy *) wl_callback);
}

#define WL_COMPOSITOR_CREATE_SURFACE 0
#define WL_COMPOSITOR_CREATE_REGION 1


/**
 * @ingroup iface_wl_compositor
 */
#define WL_COMPOSITOR_CREATE_SURFACE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_compositor
 */
#define WL_COMPOSITOR_CREATE_REGION_SINCE_VERSION 1

/** @ingroup iface_wl_compositor */
static inline void
wl_compositor_set_user_data(struct wl_compositor *wl_compositor, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_compositor, user_data);
}

/** @ingroup iface_wl_compositor */
static inline void *
wl_compositor_get_user_data(struct wl_compositor *wl_compositor)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_compositor);
}

static inline uint32_t
wl_compositor_get_version(struct wl_compositor *wl_compositor)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_compositor);
}

/** @ingroup iface_wl_compositor */
static inline void
wl_compositor_destroy(struct wl_compositor *wl_compositor)
{
	wl_proxy_destroy((struct wl_proxy *) wl_compositor);
}

/**
 * @ingroup iface_wl_compositor
 *
 * Ask the compositor to create a new surface.
 */
static inline struct wl_surface *
wl_compositor_create_surface(struct wl_compositor *wl_compositor)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) wl_compositor,
			 WL_COMPOSITOR_CREATE_SURFACE, &wl_surface_interface, NULL);

	return (struct wl_surface *) id;
}

/**
 * @ingroup iface_wl_compositor
 *
 * Ask the compositor to create a new region.
 */
static inline struct wl_region *
wl_compositor_create_region(struct wl_compositor *wl_compositor)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) wl_compositor,
			 WL_COMPOSITOR_CREATE_REGION, &wl_region_interface, NULL);

	return (struct wl_region *) id;
}

#define WL_SHM_POOL_CREATE_BUFFER 0
#define WL_SHM_POOL_DESTROY 1
#define WL_SHM_POOL_RESIZE 2


/**
 * @ingroup iface_wl_shm_pool
 */
#define WL_SHM_POOL_CREATE_BUFFER_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shm_pool
 */
#define WL_SHM_POOL_DESTROY_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shm_pool
 */
#define WL_SHM_POOL_RESIZE_SINCE_VERSION 1

/** @ingroup iface_wl_shm_pool */
static inline void
wl_shm_pool_set_user_data(struct wl_shm_pool *wl_shm_pool, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_shm_pool, user_data);
}

/** @ingroup iface_wl_shm_pool */
static inline void *
wl_shm_pool_get_user_data(struct wl_shm_pool *wl_shm_pool)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_shm_pool);
}

static inline uint32_t
wl_shm_pool_get_version(struct wl_shm_pool *wl_shm_pool)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_shm_pool);
}

/**
 * @ingroup iface_wl_shm_pool
 *
 * Create a wl_buffer object from the pool.
 *
 * The buffer is created offset bytes into the pool and has
 * width and height as specified.  The stride argument specifies
 * the number of bytes from the beginning of one row to the beginning
 * of the next.  The format is the pixel format of the buffer and
 * must be one of those advertised through the wl_shm.format event.
 *
 * A buffer will keep a reference to the pool it was created from
 * so it is valid to destroy the pool immediately after creating
 * a buffer from it.
 */
static inline struct wl_buffer *
wl_shm_pool_create_buffer(struct wl_shm_pool *wl_shm_pool, int32_t offset, int32_t width, int32_t height, int32_t stride, uint32_t format)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) wl_shm_pool,
			 WL_SHM_POOL_CREATE_BUFFER, &wl_buffer_interface, NULL, offset, width, height, stride, format);

	return (struct wl_buffer *) id;
}

/**
 * @ingroup iface_wl_shm_pool
 *
 * Destroy the shared memory pool.
 *
 * The mmapped memory will be released when all
 * buffers that have been created from this pool
 * are gone.
 */
static inline void
wl_shm_pool_destroy(struct wl_shm_pool *wl_shm_pool)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shm_pool,
			 WL_SHM_POOL_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_shm_pool);
}

/**
 * @ingroup iface_wl_shm_pool
 *
 * This request will cause the server to remap the backing memory
 * for the pool from the file descriptor passed when the pool was
 * created, but using the new size.  This request can only be
 * used to make the pool bigger.
 */
static inline void
wl_shm_pool_resize(struct wl_shm_pool *wl_shm_pool, int32_t size)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shm_pool,
			 WL_SHM_POOL_RESIZE, size);
}

#ifndef WL_SHM_ERROR_ENUM
#define WL_SHM_ERROR_ENUM
/**
 * @ingroup iface_wl_shm
 * wl_shm error values
 *
 * These errors can be emitted in response to wl_shm requests.
 */
enum wl_shm_error {
	/**
	 * buffer format is not known
	 */
	WL_SHM_ERROR_INVALID_FORMAT = 0,
	/**
	 * invalid size or stride during pool or buffer creation
	 */
	WL_SHM_ERROR_INVALID_STRIDE = 1,
	/**
	 * mmapping the file descriptor failed
	 */
	WL_SHM_ERROR_INVALID_FD = 2,
};
#endif /* WL_SHM_ERROR_ENUM */

#ifndef WL_SHM_FORMAT_ENUM
#define WL_SHM_FORMAT_ENUM
/**
 * @ingroup iface_wl_shm
 * pixel formats
 *
 * This describes the memory layout of an individual pixel.
 *
 * All renderers should support argb8888 and xrgb8888 but any other
 * formats are optional and may not be supported by the particular
 * renderer in use.
 *
 * The drm format codes match the macros defined in drm_fourcc.h.
 * The formats actually supported by the compositor will be
 * reported by the format event.
 */
enum wl_shm_format {
	/**
	 * 32-bit ARGB format, [31:0] A:R:G:B 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_ARGB8888 = 0,
	/**
	 * 32-bit RGB format, [31:0] x:R:G:B 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_XRGB8888 = 1,
	/**
	 * 8-bit color index format, [7:0] C
	 */
	WL_SHM_FORMAT_C8 = 0x20203843,
	/**
	 * 8-bit RGB format, [7:0] R:G:B 3:3:2
	 */
	WL_SHM_FORMAT_RGB332 = 0x38424752,
	/**
	 * 8-bit BGR format, [7:0] B:G:R 2:3:3
	 */
	WL_SHM_FORMAT_BGR233 = 0x38524742,
	/**
	 * 16-bit xRGB format, [15:0] x:R:G:B 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_XRGB4444 = 0x32315258,
	/**
	 * 16-bit xBGR format, [15:0] x:B:G:R 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_XBGR4444 = 0x32314258,
	/**
	 * 16-bit RGBx format, [15:0] R:G:B:x 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_RGBX4444 = 0x32315852,
	/**
	 * 16-bit BGRx format, [15:0] B:G:R:x 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_BGRX4444 = 0x32315842,
	/**
	 * 16-bit ARGB format, [15:0] A:R:G:B 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_ARGB4444 = 0x32315241,
	/**
	 * 16-bit ABGR format, [15:0] A:B:G:R 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_ABGR4444 = 0x32314241,
	/**
	 * 16-bit RBGA format, [15:0] R:G:B:A 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_RGBA4444 = 0x32314152,
	/**
	 * 16-bit BGRA format, [15:0] B:G:R:A 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_BGRA4444 = 0x32314142,
	/**
	 * 16-bit xRGB format, [15:0] x:R:G:B 1:5:5:5 little endian
	 */
	WL_SHM_FORMAT_XRGB1555 = 0x35315258,
	/**
	 * 16-bit xBGR 1555 format, [15:0] x:B:G:R 1:5:5:5 little endian
	 */
	WL_SHM_FORMAT_XBGR1555 = 0x35314258,
	/**
	 * 16-bit RGBx 5551 format, [15:0] R:G:B:x 5:5:5:1 little endian
	 */
	WL_SHM_FORMAT_RGBX5551 = 0x35315852,
	/**
	 * 16-bit BGRx 5551 format, [15:0] B:G:R:x 5:5:5:1 little endian
	 */
	WL_SHM_FORMAT_BGRX5551 = 0x35315842,
	/**
	 * 16-bit ARGB 1555 format, [15:0] A:R:G:B 1:5:5:5 little endian
	 */
	WL_SHM_FORMAT_ARGB1555 = 0x35315241,
	/**
	 * 16-bit ABGR 1555 format, [15:0] A:B:G:R 1:5:5:5 little endian
	 */
	WL_SHM_FORMAT_ABGR1555 = 0x35314241,
	/**
	 * 16-bit RGBA 5551 format, [15:0] R:G:B:A 5:5:5:1 little endian
	 */
	WL_SHM_FORMAT_RGBA5551 = 0x35314152,
	/**
	 * 16-bit BGRA 5551 format, [15:0] B:G:R:A 5:5:5:1 little endian
	 */
	WL_SHM_FORMAT_BGRA5551 = 0x35314142,
	/**
	 * 16-bit RGB 565 format, [15:0] R:G:B 5:6:5 little endian
	 */
	WL_SHM_FORMAT_RGB565 = 0x36314752,
	/**
	 * 16-bit BGR 565 format, [15:0] B:G:R 5:6:5 little endian
	 */
	WL_SHM_FORMAT_BGR565 = 0x36314742,
	/**
	 * 24-bit RGB format, [23:0] R:G:B little endian
	 */
	WL_SHM_FORMAT_RGB888 = 0x34324752,
	/**
	 * 24-bit BGR format, [23:0] B:G:R little endian
	 */
	WL_SHM_FORMAT_BGR888 = 0x34324742,
	/**
	 * 32-bit xBGR format, [31:0] x:B:G:R 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_XBGR8888 = 0x34324258,
	/**
	 * 32-bit RGBx format, [31:0] R:G:B:x 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_RGBX8888 = 0x34325852,
	/**
	 * 32-bit BGRx format, [31:0] B:G:R:x 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_BGRX8888 = 0x34325842,
	/**
	 * 32-bit ABGR format, [31:0] A:B:G:R 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_ABGR8888 = 0x34324241,
	/**
	 * 32-bit RGBA format, [31:0] R:G:B:A 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_RGBA8888 = 0x34324152,
	/**
	 * 32-bit BGRA format, [31:0] B:G:R:A 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_BGRA8888 = 0x34324142,
	/**
	 * 32-bit xRGB format, [31:0] x:R:G:B 2:10:10:10 little endian
	 */
	WL_SHM_FORMAT_XRGB2101010 = 0x30335258,
	/**
	 * 32-bit xBGR format, [31:0] x:B:G:R 2:10:10:10 little endian
	 */
	WL_SHM_FORMAT_XBGR2101010 = 0x30334258,
	/**
	 * 32-bit RGBx format, [31:0] R:G:B:x 10:10:10:2 little endian
	 */
	WL_SHM_FORMAT_RGBX1010102 = 0x30335852,
	/**
	 * 32-bit BGRx format, [31:0] B:G:R:x 10:10:10:2 little endian
	 */
	WL_SHM_FORMAT_BGRX1010102 = 0x30335842,
	/**
	 * 32-bit ARGB format, [31:0] A:R:G:B 2:10:10:10 little endian
	 */
	WL_SHM_FORMAT_ARGB2101010 = 0x30335241,
	/**
	 * 32-bit ABGR format, [31:0] A:B:G:R 2:10:10:10 little endian
	 */
	WL_SHM_FORMAT_ABGR2101010 = 0x30334241,
	/**
	 * 32-bit RGBA format, [31:0] R:G:B:A 10:10:10:2 little endian
	 */
	WL_SHM_FORMAT_RGBA1010102 = 0x30334152,
	/**
	 * 32-bit BGRA format, [31:0] B:G:R:A 10:10:10:2 little endian
	 */
	WL_SHM_FORMAT_BGRA1010102 = 0x30334142,
	/**
	 * packed YCbCr format, [31:0] Cr0:Y1:Cb0:Y0 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_YUYV = 0x56595559,
	/**
	 * packed YCbCr format, [31:0] Cb0:Y1:Cr0:Y0 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_YVYU = 0x55595659,
	/**
	 * packed YCbCr format, [31:0] Y1:Cr0:Y0:Cb0 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_UYVY = 0x59565955,
	/**
	 * packed YCbCr format, [31:0] Y1:Cb0:Y0:Cr0 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_VYUY = 0x59555956,
	/**
	 * packed AYCbCr format, [31:0] A:Y:Cb:Cr 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_AYUV = 0x56555941,
	/**
	 * 2 plane YCbCr Cr:Cb format, 2x2 subsampled Cr:Cb plane
	 */
	WL_SHM_FORMAT_NV12 = 0x3231564e,
	/**
	 * 2 plane YCbCr Cb:Cr format, 2x2 subsampled Cb:Cr plane
	 */
	WL_SHM_FORMAT_NV21 = 0x3132564e,
	/**
	 * 2 plane YCbCr Cr:Cb format, 2x1 subsampled Cr:Cb plane
	 */
	WL_SHM_FORMAT_NV16 = 0x3631564e,
	/**
	 * 2 plane YCbCr Cb:Cr format, 2x1 subsampled Cb:Cr plane
	 */
	WL_SHM_FORMAT_NV61 = 0x3136564e,
	/**
	 * 3 plane YCbCr format, 4x4 subsampled Cb (1) and Cr (2) planes
	 */
	WL_SHM_FORMAT_YUV410 = 0x39565559,
	/**
	 * 3 plane YCbCr format, 4x4 subsampled Cr (1) and Cb (2) planes
	 */
	WL_SHM_FORMAT_YVU410 = 0x39555659,
	/**
	 * 3 plane YCbCr format, 4x1 subsampled Cb (1) and Cr (2) planes
	 */
	WL_SHM_FORMAT_YUV411 = 0x31315559,
	/**
	 * 3 plane YCbCr format, 4x1 subsampled Cr (1) and Cb (2) planes
	 */
	WL_SHM_FORMAT_YVU411 = 0x31315659,
	/**
	 * 3 plane YCbCr format, 2x2 subsampled Cb (1) and Cr (2) planes
	 */
	WL_SHM_FORMAT_YUV420 = 0x32315559,
	/**
	 * 3 plane YCbCr format, 2x2 subsampled Cr (1) and Cb (2) planes
	 */
	WL_SHM_FORMAT_YVU420 = 0x32315659,
	/**
	 * 3 plane YCbCr format, 2x1 subsampled Cb (1) and Cr (2) planes
	 */
	WL_SHM_FORMAT_YUV422 = 0x36315559,
	/**
	 * 3 plane YCbCr format, 2x1 subsampled Cr (1) and Cb (2) planes
	 */
	WL_SHM_FORMAT_YVU422 = 0x36315659,
	/**
	 * 3 plane YCbCr format, non-subsampled Cb (1) and Cr (2) planes
	 */
	WL_SHM_FORMAT_YUV444 = 0x34325559,
	/**
	 * 3 plane YCbCr format, non-subsampled Cr (1) and Cb (2) planes
	 */
	WL_SHM_FORMAT_YVU444 = 0x34325659,
};
#endif /* WL_SHM_FORMAT_ENUM */

/**
 * @ingroup iface_wl_shm
 * @struct wl_shm_listener
 */
struct wl_shm_listener {
	/**
	 * pixel format description
	 *
	 * Informs the client about a valid pixel format that can be used
	 * for buffers. Known formats include argb8888 and xrgb8888.
	 * @param format buffer pixel format
	 */
	void (*format)(void *data,
		       struct wl_shm *wl_shm,
		       uint32_t format);
};

/**
 * @ingroup iface_wl_shm
 */
static inline int
wl_shm_add_listener(struct wl_shm *wl_shm,
		    const struct wl_shm_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_shm,
				     (void (**)(void)) listener, data);
}

#define WL_SHM_CREATE_POOL 0

/**
 * @ingroup iface_wl_shm
 */
#define WL_SHM_FORMAT_SINCE_VERSION 1

/**
 * @ingroup iface_wl_shm
 */
#define WL_SHM_CREATE_POOL_SINCE_VERSION 1

/** @ingroup iface_wl_shm */
static inline void
wl_shm_set_user_data(struct wl_shm *wl_shm, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_shm, user_data);
}

/** @ingroup iface_wl_shm */
static inline void *
wl_shm_get_user_data(struct wl_shm *wl_shm)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_shm);
}

static inline uint32_t
wl_shm_get_version(struct wl_shm *wl_shm)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_shm);
}

/** @ingroup iface_wl_shm */
static inline void
wl_shm_destroy(struct wl_shm *wl_shm)
{
	wl_proxy_destroy((struct wl_proxy *) wl_shm);
}

/**
 * @ingroup iface_wl_shm
 *
 * Create a new wl_shm_pool object.
 *
 * The pool can be used to create shared memory based buffer
 * objects.  The server will mmap size bytes of the passed file
 * descriptor, to use as backing memory for the pool.
 */
static inline struct wl_shm_pool *
wl_shm_create_pool(struct wl_shm *wl_shm, int32_t fd, int32_t size)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) wl_shm,
			 WL_SHM_CREATE_POOL, &wl_shm_pool_interface, NULL, fd, size);

	return (struct wl_shm_pool *) id;
}

/**
 * @ingroup iface_wl_buffer
 * @struct wl_buffer_listener
 */
struct wl_buffer_listener {
	/**
	 * compositor releases buffer
	 *
	 * Sent when this wl_buffer is no longer used by the compositor.
	 * The client is now free to reuse or destroy this buffer and its
	 * backing storage.
	 *
	 * If a client receives a release event before the frame callback
	 * requested in the same wl_surface.commit that attaches this
	 * wl_buffer to a surface, then the client is immediately free to
	 * reuse the buffer and its backing storage, and does not need a
	 * second buffer for the next surface content update. Typically
	 * this is possible, when the compositor maintains a copy of the
	 * wl_surface contents, e.g. as a GL texture. This is an important
	 * optimization for GL(ES) compositors with wl_shm clients.
	 */
	void (*release)(void *data,
			struct wl_buffer *wl_buffer);
};

/**
 * @ingroup iface_wl_buffer
 */
static inline int
wl_buffer_add_listener(struct wl_buffer *wl_buffer,
		       const struct wl_buffer_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_buffer,
				     (void (**)(void)) listener, data);
}

#define WL_BUFFER_DESTROY 0

/**
 * @ingroup iface_wl_buffer
 */
#define WL_BUFFER_RELEASE_SINCE_VERSION 1

/**
 * @ingroup iface_wl_buffer
 */
#define WL_BUFFER_DESTROY_SINCE_VERSION 1

/** @ingroup iface_wl_buffer */
static inline void
wl_buffer_set_user_data(struct wl_buffer *wl_buffer, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_buffer, user_data);
}

/** @ingroup iface_wl_buffer */
static inline void *
wl_buffer_get_user_data(struct wl_buffer *wl_buffer)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_buffer);
}

static inline uint32_t
wl_buffer_get_version(struct wl_buffer *wl_buffer)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_buffer);
}

/**
 * @ingroup iface_wl_buffer
 *
 * Destroy a buffer. If and how you need to release the backing
 * storage is defined by the buffer factory interface.
 *
 * For possible side-effects to a surface, see wl_surface.attach.
 */
static inline void
wl_buffer_destroy(struct wl_buffer *wl_buffer)
{
	wl_proxy_marshal((struct wl_proxy *) wl_buffer,
			 WL_BUFFER_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_buffer);
}

#ifndef WL_DATA_OFFER_ERROR_ENUM
#define WL_DATA_OFFER_ERROR_ENUM
enum wl_data_offer_error {
	/**
	 * finish request was called untimely
	 */
	WL_DATA_OFFER_ERROR_INVALID_FINISH = 0,
	/**
	 * action mask contains invalid values
	 */
	WL_DATA_OFFER_ERROR_INVALID_ACTION_MASK = 1,
	/**
	 * action argument has an invalid value
	 */
	WL_DATA_OFFER_ERROR_INVALID_ACTION = 2,
	/**
	 * offer doesn't accept this request
	 */
	WL_DATA_OFFER_ERROR_INVALID_OFFER = 3,
};
#endif /* WL_DATA_OFFER_ERROR_ENUM */

/**
 * @ingroup iface_wl_data_offer
 * @struct wl_data_offer_listener
 */
struct wl_data_offer_listener {
	/**
	 * advertise offered mime type
	 *
	 * Sent immediately after creating the wl_data_offer object. One
	 * event per offered mime type.
	 * @param mime_type offered mime type
	 */
	void (*offer)(void *data,
		      struct wl_data_offer *wl_data_offer,
		      const char *mime_type);
	/**
	 * notify the source-side available actions
	 *
	 * This event indicates the actions offered by the data source.
	 * It will be sent right after wl_data_device.enter, or anytime the
	 * source side changes its offered actions through
	 * wl_data_source.set_actions.
	 * @param source_actions actions offered by the data source
	 * @since 3
	 */
	void (*source_actions)(void *data,
			       struct wl_data_offer *wl_data_offer,
			       uint32_t source_actions);
	/**
	 * notify the selected action
	 *
	 * This event indicates the action selected by the compositor
	 * after matching the source/destination side actions. Only one
	 * action (or none) will be offered here.
	 *
	 * This event can be emitted multiple times during the
	 * drag-and-drop operation in response to destination side action
	 * changes through wl_data_offer.set_actions.
	 *
	 * This event will no longer be emitted after wl_data_device.drop
	 * happened on the drag-and-drop destination, the client must honor
	 * the last action received, or the last preferred one set through
	 * wl_data_offer.set_actions when handling an "ask" action.
	 *
	 * Compositors may also change the selected action on the fly,
	 * mainly in response to keyboard modifier changes during the
	 * drag-and-drop operation.
	 *
	 * The most recent action received is always the valid one. Prior
	 * to receiving wl_data_device.drop, the chosen action may change
	 * (e.g. due to keyboard modifiers being pressed). At the time of
	 * receiving wl_data_device.drop the drag-and-drop destination must
	 * honor the last action received.
	 *
	 * Action changes may still happen after wl_data_device.drop,
	 * especially on "ask" actions, where the drag-and-drop destination
	 * may choose another action afterwards. Action changes happening
	 * at this stage are always the result of inter-client negotiation,
	 * the compositor shall no longer be able to induce a different
	 * action.
	 *
	 * Upon "ask" actions, it is expected that the drag-and-drop
	 * destination may potentially choose a different action and/or
	 * mime type, based on wl_data_offer.source_actions and finally
	 * chosen by the user (e.g. popping up a menu with the available
	 * options). The final wl_data_offer.set_actions and
	 * wl_data_offer.accept requests must happen before the call to
	 * wl_data_offer.finish.
	 * @param dnd_action action selected by the compositor
	 * @since 3
	 */
	void (*action)(void *data,
		       struct wl_data_offer *wl_data_offer,
		       uint32_t dnd_action);
};

/**
 * @ingroup iface_wl_data_offer
 */
static inline int
wl_data_offer_add_listener(struct wl_data_offer *wl_data_offer,
			   const struct wl_data_offer_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_data_offer,
				     (void (**)(void)) listener, data);
}

#define WL_DATA_OFFER_ACCEPT 0
#define WL_DATA_OFFER_RECEIVE 1
#define WL_DATA_OFFER_DESTROY 2
#define WL_DATA_OFFER_FINISH 3
#define WL_DATA_OFFER_SET_ACTIONS 4

/**
 * @ingroup iface_wl_data_offer
 */
#define WL_DATA_OFFER_OFFER_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_offer
 */
#define WL_DATA_OFFER_SOURCE_ACTIONS_SINCE_VERSION 3
/**
 * @ingroup iface_wl_data_offer
 */
#define WL_DATA_OFFER_ACTION_SINCE_VERSION 3

/**
 * @ingroup iface_wl_data_offer
 */
#define WL_DATA_OFFER_ACCEPT_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_offer
 */
#define WL_DATA_OFFER_RECEIVE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_offer
 */
#define WL_DATA_OFFER_DESTROY_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_offer
 */
#define WL_DATA_OFFER_FINISH_SINCE_VERSION 3
/**
 * @ingroup iface_wl_data_offer
 */
#define WL_DATA_OFFER_SET_ACTIONS_SINCE_VERSION 3

/** @ingroup iface_wl_data_offer */
static inline void
wl_data_offer_set_user_data(struct wl_data_offer *wl_data_offer, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_data_offer, user_data);
}

/** @ingroup iface_wl_data_offer */
static inline void *
wl_data_offer_get_user_data(struct wl_data_offer *wl_data_offer)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_data_offer);
}

static inline uint32_t
wl_data_offer_get_version(struct wl_data_offer *wl_data_offer)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_data_offer);
}

/**
 * @ingroup iface_wl_data_offer
 *
 * Indicate that the client can accept the given mime type, or
 * NULL for not accepted.
 *
 * For objects of version 2 or older, this request is used by the
 * client to give feedback whether the client can receive the given
 * mime type, or NULL if none is accepted; the feedback does not
 * determine whether the drag-and-drop operation succeeds or not.
 *
 * For objects of version 3 or newer, this request determines the
 * final result of the drag-and-drop operation. If the end result
 * is that no mime types were accepted, the drag-and-drop operation
 * will be cancelled and the corresponding drag source will receive
 * wl_data_source.cancelled. Clients may still use this event in
 * conjunction with wl_data_source.action for feedback.
 */
static inline void
wl_data_offer_accept(struct wl_data_offer *wl_data_offer, uint32_t serial, const char *mime_type)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_offer,
			 WL_DATA_OFFER_ACCEPT, serial, mime_type);
}

/**
 * @ingroup iface_wl_data_offer
 *
 * To transfer the offered data, the client issues this request
 * and indicates the mime type it wants to receive.  The transfer
 * happens through the passed file descriptor (typically created
 * with the pipe system call).  The source client writes the data
 * in the mime type representation requested and then closes the
 * file descriptor.
 *
 * The receiving client reads from the read end of the pipe until
 * EOF and then closes its end, at which point the transfer is
 * complete.
 *
 * This request may happen multiple times for different mime types,
 * both before and after wl_data_device.drop. Drag-and-drop destination
 * clients may preemptively fetch data or examine it more closely to
 * determine acceptance.
 */
static inline void
wl_data_offer_receive(struct wl_data_offer *wl_data_offer, const char *mime_type, int32_t fd)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_offer,
			 WL_DATA_OFFER_RECEIVE, mime_type, fd);
}

/**
 * @ingroup iface_wl_data_offer
 *
 * Destroy the data offer.
 */
static inline void
wl_data_offer_destroy(struct wl_data_offer *wl_data_offer)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_offer,
			 WL_DATA_OFFER_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_data_offer);
}

/**
 * @ingroup iface_wl_data_offer
 *
 * Notifies the compositor that the drag destination successfully
 * finished the drag-and-drop operation.
 *
 * Upon receiving this request, the compositor will emit
 * wl_data_source.dnd_finished on the drag source client.
 *
 * It is a client error to perform other requests than
 * wl_data_offer.destroy after this one. It is also an error to perform
 * this request after a NULL mime type has been set in
 * wl_data_offer.accept or no action was received through
 * wl_data_offer.action.
 */
static inline void
wl_data_offer_finish(struct wl_data_offer *wl_data_offer)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_offer,
			 WL_DATA_OFFER_FINISH);
}

/**
 * @ingroup iface_wl_data_offer
 *
 * Sets the actions that the destination side client supports for
 * this operation. This request may trigger the emission of
 * wl_data_source.action and wl_data_offer.action events if the compositor
 * needs to change the selected action.
 *
 * This request can be called multiple times throughout the
 * drag-and-drop operation, typically in response to wl_data_device.enter
 * or wl_data_device.motion events.
 *
 * This request determines the final result of the drag-and-drop
 * operation. If the end result is that no action is accepted,
 * the drag source will receive wl_drag_source.cancelled.
 *
 * The dnd_actions argument must contain only values expressed in the
 * wl_data_device_manager.dnd_actions enum, and the preferred_action
 * argument must only contain one of those values set, otherwise it
 * will result in a protocol error.
 *
 * While managing an "ask" action, the destination drag-and-drop client
 * may perform further wl_data_offer.receive requests, and is expected
 * to perform one last wl_data_offer.set_actions request with a preferred
 * action other than "ask" (and optionally wl_data_offer.accept) before
 * requesting wl_data_offer.finish, in order to convey the action selected
 * by the user. If the preferred action is not in the
 * wl_data_offer.source_actions mask, an error will be raised.
 *
 * If the "ask" action is dismissed (e.g. user cancellation), the client
 * is expected to perform wl_data_offer.destroy right away.
 *
 * This request can only be made on drag-and-drop offers, a protocol error
 * will be raised otherwise.
 */
static inline void
wl_data_offer_set_actions(struct wl_data_offer *wl_data_offer, uint32_t dnd_actions, uint32_t preferred_action)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_offer,
			 WL_DATA_OFFER_SET_ACTIONS, dnd_actions, preferred_action);
}

#ifndef WL_DATA_SOURCE_ERROR_ENUM
#define WL_DATA_SOURCE_ERROR_ENUM
enum wl_data_source_error {
	/**
	 * action mask contains invalid values
	 */
	WL_DATA_SOURCE_ERROR_INVALID_ACTION_MASK = 0,
	/**
	 * source doesn't accept this request
	 */
	WL_DATA_SOURCE_ERROR_INVALID_SOURCE = 1,
};
#endif /* WL_DATA_SOURCE_ERROR_ENUM */

/**
 * @ingroup iface_wl_data_source
 * @struct wl_data_source_listener
 */
struct wl_data_source_listener {
	/**
	 * a target accepts an offered mime type
	 *
	 * Sent when a target accepts pointer_focus or motion events. If
	 * a target does not accept any of the offered types, type is NULL.
	 *
	 * Used for feedback during drag-and-drop.
	 * @param mime_type mime type accepted by the target
	 */
	void (*target)(void *data,
		       struct wl_data_source *wl_data_source,
		       const char *mime_type);
	/**
	 * send the data
	 *
	 * Request for data from the client. Send the data as the
	 * specified mime type over the passed file descriptor, then close
	 * it.
	 * @param mime_type mime type for the data
	 * @param fd file descriptor for the data
	 */
	void (*send)(void *data,
		     struct wl_data_source *wl_data_source,
		     const char *mime_type,
		     int32_t fd);
	/**
	 * selection was cancelled
	 *
	 * This data source is no longer valid. There are several reasons
	 * why this could happen:
	 *
	 * - The data source has been replaced by another data source. -
	 * The drag-and-drop operation was performed, but the drop
	 * destination did not accept any of the mime types offered through
	 * wl_data_source.target. - The drag-and-drop operation was
	 * performed, but the drop destination did not select any of the
	 * actions present in the mask offered through
	 * wl_data_source.action. - The drag-and-drop operation was
	 * performed but didn't happen over a surface. - The compositor
	 * cancelled the drag-and-drop operation (e.g. compositor dependent
	 * timeouts to avoid stale drag-and-drop transfers).
	 *
	 * The client should clean up and destroy this data source.
	 *
	 * For objects of version 2 or older, wl_data_source.cancelled will
	 * only be emitted if the data source was replaced by another data
	 * source.
	 */
	void (*cancelled)(void *data,
			  struct wl_data_source *wl_data_source);
	/**
	 * the drag-and-drop operation physically finished
	 *
	 * The user performed the drop action. This event does not
	 * indicate acceptance, wl_data_source.cancelled may still be
	 * emitted afterwards if the drop destination does not accept any
	 * mime type.
	 *
	 * However, this event might however not be received if the
	 * compositor cancelled the drag-and-drop operation before this
	 * event could happen.
	 *
	 * Note that the data_source may still be used in the future and
	 * should not be destroyed here.
	 * @since 3
	 */
	void (*dnd_drop_performed)(void *data,
				   struct wl_data_source *wl_data_source);
	/**
	 * the drag-and-drop operation concluded
	 *
	 * The drop destination finished interoperating with this data
	 * source, so the client is now free to destroy this data source
	 * and free all associated data.
	 *
	 * If the action used to perform the operation was "move", the
	 * source can now delete the transferred data.
	 * @since 3
	 */
	void (*dnd_finished)(void *data,
			     struct wl_data_source *wl_data_source);
	/**
	 * notify the selected action
	 *
	 * This event indicates the action selected by the compositor
	 * after matching the source/destination side actions. Only one
	 * action (or none) will be offered here.
	 *
	 * This event can be emitted multiple times during the
	 * drag-and-drop operation, mainly in response to destination side
	 * changes through wl_data_offer.set_actions, and as the data
	 * device enters/leaves surfaces.
	 *
	 * It is only possible to receive this event after
	 * wl_data_source.dnd_drop_performed if the drag-and-drop operation
	 * ended in an "ask" action, in which case the final
	 * wl_data_source.action event will happen immediately before
	 * wl_data_source.dnd_finished.
	 *
	 * Compositors may also change the selected action on the fly,
	 * mainly in response to keyboard modifier changes during the
	 * drag-and-drop operation.
	 *
	 * The most recent action received is always the valid one. The
	 * chosen action may change alongside negotiation (e.g. an "ask"
	 * action can turn into a "move" operation), so the effects of the
	 * final action must always be applied in
	 * wl_data_offer.dnd_finished.
	 *
	 * Clients can trigger cursor surface changes from this point, so
	 * they reflect the current action.
	 * @param dnd_action action selected by the compositor
	 * @since 3
	 */
	void (*action)(void *data,
		       struct wl_data_source *wl_data_source,
		       uint32_t dnd_action);
};

/**
 * @ingroup iface_wl_data_source
 */
static inline int
wl_data_source_add_listener(struct wl_data_source *wl_data_source,
			    const struct wl_data_source_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_data_source,
				     (void (**)(void)) listener, data);
}

#define WL_DATA_SOURCE_OFFER 0
#define WL_DATA_SOURCE_DESTROY 1
#define WL_DATA_SOURCE_SET_ACTIONS 2

/**
 * @ingroup iface_wl_data_source
 */
#define WL_DATA_SOURCE_TARGET_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_source
 */
#define WL_DATA_SOURCE_SEND_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_source
 */
#define WL_DATA_SOURCE_CANCELLED_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_source
 */
#define WL_DATA_SOURCE_DND_DROP_PERFORMED_SINCE_VERSION 3
/**
 * @ingroup iface_wl_data_source
 */
#define WL_DATA_SOURCE_DND_FINISHED_SINCE_VERSION 3
/**
 * @ingroup iface_wl_data_source
 */
#define WL_DATA_SOURCE_ACTION_SINCE_VERSION 3

/**
 * @ingroup iface_wl_data_source
 */
#define WL_DATA_SOURCE_OFFER_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_source
 */
#define WL_DATA_SOURCE_DESTROY_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_source
 */
#define WL_DATA_SOURCE_SET_ACTIONS_SINCE_VERSION 3

/** @ingroup iface_wl_data_source */
static inline void
wl_data_source_set_user_data(struct wl_data_source *wl_data_source, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_data_source, user_data);
}

/** @ingroup iface_wl_data_source */
static inline void *
wl_data_source_get_user_data(struct wl_data_source *wl_data_source)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_data_source);
}

static inline uint32_t
wl_data_source_get_version(struct wl_data_source *wl_data_source)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_data_source);
}

/**
 * @ingroup iface_wl_data_source
 *
 * This request adds a mime type to the set of mime types
 * advertised to targets.  Can be called several times to offer
 * multiple types.
 */
static inline void
wl_data_source_offer(struct wl_data_source *wl_data_source, const char *mime_type)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_source,
			 WL_DATA_SOURCE_OFFER, mime_type);
}

/**
 * @ingroup iface_wl_data_source
 *
 * Destroy the data source.
 */
static inline void
wl_data_source_destroy(struct wl_data_source *wl_data_source)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_source,
			 WL_DATA_SOURCE_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_data_source);
}

/**
 * @ingroup iface_wl_data_source
 *
 * Sets the actions that the source side client supports for this
 * operation. This request may trigger wl_data_source.action and
 * wl_data_offer.action events if the compositor needs to change the
 * selected action.
 *
 * The dnd_actions argument must contain only values expressed in the
 * wl_data_device_manager.dnd_actions enum, otherwise it will result
 * in a protocol error.
 *
 * This request must be made once only, and can only be made on sources
 * used in drag-and-drop, so it must be performed before
 * wl_data_device.start_drag. Attempting to use the source other than
 * for drag-and-drop will raise a protocol error.
 */
static inline void
wl_data_source_set_actions(struct wl_data_source *wl_data_source, uint32_t dnd_actions)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_source,
			 WL_DATA_SOURCE_SET_ACTIONS, dnd_actions);
}

#ifndef WL_DATA_DEVICE_ERROR_ENUM
#define WL_DATA_DEVICE_ERROR_ENUM
enum wl_data_device_error {
	/**
	 * given wl_surface has another role
	 */
	WL_DATA_DEVICE_ERROR_ROLE = 0,
};
#endif /* WL_DATA_DEVICE_ERROR_ENUM */

/**
 * @ingroup iface_wl_data_device
 * @struct wl_data_device_listener
 */
struct wl_data_device_listener {
	/**
	 * introduce a new wl_data_offer
	 *
	 * The data_offer event introduces a new wl_data_offer object,
	 * which will subsequently be used in either the data_device.enter
	 * event (for drag-and-drop) or the data_device.selection event
	 * (for selections). Immediately following the
	 * data_device_data_offer event, the new data_offer object will
	 * send out data_offer.offer events to describe the mime types it
	 * offers.
	 * @param id the new data_offer object
	 */
	void (*data_offer)(void *data,
			   struct wl_data_device *wl_data_device,
			   struct wl_data_offer *id);
	/**
	 * initiate drag-and-drop session
	 *
	 * This event is sent when an active drag-and-drop pointer enters
	 * a surface owned by the client. The position of the pointer at
	 * enter time is provided by the x and y arguments, in
	 * surface-local coordinates.
	 * @param serial serial number of the enter event
	 * @param surface client surface entered
	 * @param x surface-local x coordinate
	 * @param y surface-local y coordinate
	 * @param id source data_offer object
	 */
	void (*enter)(void *data,
		      struct wl_data_device *wl_data_device,
		      uint32_t serial,
		      struct wl_surface *surface,
		      wl_fixed_t x,
		      wl_fixed_t y,
		      struct wl_data_offer *id);
	/**
	 * end drag-and-drop session
	 *
	 * This event is sent when the drag-and-drop pointer leaves the
	 * surface and the session ends. The client must destroy the
	 * wl_data_offer introduced at enter time at this point.
	 */
	void (*leave)(void *data,
		      struct wl_data_device *wl_data_device);
	/**
	 * drag-and-drop session motion
	 *
	 * This event is sent when the drag-and-drop pointer moves within
	 * the currently focused surface. The new position of the pointer
	 * is provided by the x and y arguments, in surface-local
	 * coordinates.
	 * @param time timestamp with millisecond granularity
	 * @param x surface-local x coordinate
	 * @param y surface-local y coordinate
	 */
	void (*motion)(void *data,
		       struct wl_data_device *wl_data_device,
		       uint32_t time,
		       wl_fixed_t x,
		       wl_fixed_t y);
	/**
	 * end drag-and-drop session successfully
	 *
	 * The event is sent when a drag-and-drop operation is ended
	 * because the implicit grab is removed.
	 *
	 * The drag-and-drop destination is expected to honor the last
	 * action received through wl_data_offer.action, if the resulting
	 * action is "copy" or "move", the destination can still perform
	 * wl_data_offer.receive requests, and is expected to end all
	 * transfers with a wl_data_offer.finish request.
	 *
	 * If the resulting action is "ask", the action will not be
	 * considered final. The drag-and-drop destination is expected to
	 * perform one last wl_data_offer.set_actions request, or
	 * wl_data_offer.destroy in order to cancel the operation.
	 */
	void (*drop)(void *data,
		     struct wl_data_device *wl_data_device);
	/**
	 * advertise new selection
	 *
	 * The selection event is sent out to notify the client of a new
	 * wl_data_offer for the selection for this device. The
	 * data_device.data_offer and the data_offer.offer events are sent
	 * out immediately before this event to introduce the data offer
	 * object. The selection event is sent to a client immediately
	 * before receiving keyboard focus and when a new selection is set
	 * while the client has keyboard focus. The data_offer is valid
	 * until a new data_offer or NULL is received or until the client
	 * loses keyboard focus. The client must destroy the previous
	 * selection data_offer, if any, upon receiving this event.
	 * @param id selection data_offer object
	 */
	void (*selection)(void *data,
			  struct wl_data_device *wl_data_device,
			  struct wl_data_offer *id);
};

/**
 * @ingroup iface_wl_data_device
 */
static inline int
wl_data_device_add_listener(struct wl_data_device *wl_data_device,
			    const struct wl_data_device_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_data_device,
				     (void (**)(void)) listener, data);
}

#define WL_DATA_DEVICE_START_DRAG 0
#define WL_DATA_DEVICE_SET_SELECTION 1
#define WL_DATA_DEVICE_RELEASE 2

/**
 * @ingroup iface_wl_data_device
 */
#define WL_DATA_DEVICE_DATA_OFFER_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_device
 */
#define WL_DATA_DEVICE_ENTER_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_device
 */
#define WL_DATA_DEVICE_LEAVE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_device
 */
#define WL_DATA_DEVICE_MOTION_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_device
 */
#define WL_DATA_DEVICE_DROP_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_device
 */
#define WL_DATA_DEVICE_SELECTION_SINCE_VERSION 1

/**
 * @ingroup iface_wl_data_device
 */
#define WL_DATA_DEVICE_START_DRAG_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_device
 */
#define WL_DATA_DEVICE_SET_SELECTION_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_device
 */
#define WL_DATA_DEVICE_RELEASE_SINCE_VERSION 2

/** @ingroup iface_wl_data_device */
static inline void
wl_data_device_set_user_data(struct wl_data_device *wl_data_device, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_data_device, user_data);
}

/** @ingroup iface_wl_data_device */
static inline void *
wl_data_device_get_user_data(struct wl_data_device *wl_data_device)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_data_device);
}

static inline uint32_t
wl_data_device_get_version(struct wl_data_device *wl_data_device)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_data_device);
}

/** @ingroup iface_wl_data_device */
static inline void
wl_data_device_destroy(struct wl_data_device *wl_data_device)
{
	wl_proxy_destroy((struct wl_proxy *) wl_data_device);
}

/**
 * @ingroup iface_wl_data_device
 *
 * This request asks the compositor to start a drag-and-drop
 * operation on behalf of the client.
 *
 * The source argument is the data source that provides the data
 * for the eventual data transfer. If source is NULL, enter, leave
 * and motion events are sent only to the client that initiated the
 * drag and the client is expected to handle the data passing
 * internally.
 *
 * The origin surface is the surface where the drag originates and
 * the client must have an active implicit grab that matches the
 * serial.
 *
 * The icon surface is an optional (can be NULL) surface that
 * provides an icon to be moved around with the cursor.  Initially,
 * the top-left corner of the icon surface is placed at the cursor
 * hotspot, but subsequent wl_surface.attach request can move the
 * relative position. Attach requests must be confirmed with
 * wl_surface.commit as usual. The icon surface is given the role of
 * a drag-and-drop icon. If the icon surface already has another role,
 * it raises a protocol error.
 *
 * The current and pending input regions of the icon wl_surface are
 * cleared, and wl_surface.set_input_region is ignored until the
 * wl_surface is no longer used as the icon surface. When the use
 * as an icon ends, the current and pending input regions become
 * undefined, and the wl_surface is unmapped.
 */
static inline void
wl_data_device_start_drag(struct wl_data_device *wl_data_device, struct wl_data_source *source, struct wl_surface *origin, struct wl_surface *icon, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_device,
			 WL_DATA_DEVICE_START_DRAG, source, origin, icon, serial);
}

/**
 * @ingroup iface_wl_data_device
 *
 * This request asks the compositor to set the selection
 * to the data from the source on behalf of the client.
 *
 * To unset the selection, set the source to NULL.
 */
static inline void
wl_data_device_set_selection(struct wl_data_device *wl_data_device, struct wl_data_source *source, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_device,
			 WL_DATA_DEVICE_SET_SELECTION, source, serial);
}

/**
 * @ingroup iface_wl_data_device
 *
 * This request destroys the data device.
 */
static inline void
wl_data_device_release(struct wl_data_device *wl_data_device)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_device,
			 WL_DATA_DEVICE_RELEASE);

	wl_proxy_destroy((struct wl_proxy *) wl_data_device);
}

#ifndef WL_DATA_DEVICE_MANAGER_DND_ACTION_ENUM
#define WL_DATA_DEVICE_MANAGER_DND_ACTION_ENUM
/**
 * @ingroup iface_wl_data_device_manager
 * drag and drop actions
 *
 * This is a bitmask of the available/preferred actions in a
 * drag-and-drop operation.
 *
 * In the compositor, the selected action is a result of matching the
 * actions offered by the source and destination sides.  "action" events
 * with a "none" action will be sent to both source and destination if
 * there is no match. All further checks will effectively happen on
 * (source actions ∩ destination actions).
 *
 * In addition, compositors may also pick different actions in
 * reaction to key modifiers being pressed. One common design that
 * is used in major toolkits (and the behavior recommended for
 * compositors) is:
 *
 * - If no modifiers are pressed, the first match (in bit order)
 * will be used.
 * - Pressing Shift selects "move", if enabled in the mask.
 * - Pressing Control selects "copy", if enabled in the mask.
 *
 * Behavior beyond that is considered implementation-dependent.
 * Compositors may for example bind other modifiers (like Alt/Meta)
 * or drags initiated with other buttons than BTN_LEFT to specific
 * actions (e.g. "ask").
 */
enum wl_data_device_manager_dnd_action {
	/**
	 * no action
	 */
	WL_DATA_DEVICE_MANAGER_DND_ACTION_NONE = 0,
	/**
	 * copy action
	 */
	WL_DATA_DEVICE_MANAGER_DND_ACTION_COPY = 1,
	/**
	 * move action
	 */
	WL_DATA_DEVICE_MANAGER_DND_ACTION_MOVE = 2,
	/**
	 * ask action
	 */
	WL_DATA_DEVICE_MANAGER_DND_ACTION_ASK = 4,
};
#endif /* WL_DATA_DEVICE_MANAGER_DND_ACTION_ENUM */

#define WL_DATA_DEVICE_MANAGER_CREATE_DATA_SOURCE 0
#define WL_DATA_DEVICE_MANAGER_GET_DATA_DEVICE 1


/**
 * @ingroup iface_wl_data_device_manager
 */
#define WL_DATA_DEVICE_MANAGER_CREATE_DATA_SOURCE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_data_device_manager
 */
#define WL_DATA_DEVICE_MANAGER_GET_DATA_DEVICE_SINCE_VERSION 1

/** @ingroup iface_wl_data_device_manager */
static inline void
wl_data_device_manager_set_user_data(struct wl_data_device_manager *wl_data_device_manager, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_data_device_manager, user_data);
}

/** @ingroup iface_wl_data_device_manager */
static inline void *
wl_data_device_manager_get_user_data(struct wl_data_device_manager *wl_data_device_manager)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_data_device_manager);
}

static inline uint32_t
wl_data_device_manager_get_version(struct wl_data_device_manager *wl_data_device_manager)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_data_device_manager);
}

/** @ingroup iface_wl_data_device_manager */
static inline void
wl_data_device_manager_destroy(struct wl_data_device_manager *wl_data_device_manager)
{
	wl_proxy_destroy((struct wl_proxy *) wl_data_device_manager);
}

/**
 * @ingroup iface_wl_data_device_manager
 *
 * Create a new data source.
 */
static inline struct wl_data_source *
wl_data_device_manager_create_data_source(struct wl_data_device_manager *wl_data_device_manager)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) wl_data_device_manager,
			 WL_DATA_DEVICE_MANAGER_CREATE_DATA_SOURCE, &wl_data_source_interface, NULL);

	return (struct wl_data_source *) id;
}

/**
 * @ingroup iface_wl_data_device_manager
 *
 * Create a new data device for a given seat.
 */
static inline struct wl_data_device *
wl_data_device_manager_get_data_device(struct wl_data_device_manager *wl_data_device_manager, struct wl_seat *seat)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) wl_data_device_manager,
			 WL_DATA_DEVICE_MANAGER_GET_DATA_DEVICE, &wl_data_device_interface, NULL, seat);

	return (struct wl_data_device *) id;
}

#ifndef WL_SHELL_ERROR_ENUM
#define WL_SHELL_ERROR_ENUM
enum wl_shell_error {
	/**
	 * given wl_surface has another role
	 */
	WL_SHELL_ERROR_ROLE = 0,
};
#endif /* WL_SHELL_ERROR_ENUM */

#define WL_SHELL_GET_SHELL_SURFACE 0


/**
 * @ingroup iface_wl_shell
 */
#define WL_SHELL_GET_SHELL_SURFACE_SINCE_VERSION 1

/** @ingroup iface_wl_shell */
static inline void
wl_shell_set_user_data(struct wl_shell *wl_shell, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_shell, user_data);
}

/** @ingroup iface_wl_shell */
static inline void *
wl_shell_get_user_data(struct wl_shell *wl_shell)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_shell);
}

static inline uint32_t
wl_shell_get_version(struct wl_shell *wl_shell)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_shell);
}

/** @ingroup iface_wl_shell */
static inline void
wl_shell_destroy(struct wl_shell *wl_shell)
{
	wl_proxy_destroy((struct wl_proxy *) wl_shell);
}

/**
 * @ingroup iface_wl_shell
 *
 * Create a shell surface for an existing surface. This gives
 * the wl_surface the role of a shell surface. If the wl_surface
 * already has another role, it raises a protocol error.
 *
 * Only one shell surface can be associated with a given surface.
 */
static inline struct wl_shell_surface *
wl_shell_get_shell_surface(struct wl_shell *wl_shell, struct wl_surface *surface)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) wl_shell,
			 WL_SHELL_GET_SHELL_SURFACE, &wl_shell_surface_interface, NULL, surface);

	return (struct wl_shell_surface *) id;
}

#ifndef WL_SHELL_SURFACE_RESIZE_ENUM
#define WL_SHELL_SURFACE_RESIZE_ENUM
/**
 * @ingroup iface_wl_shell_surface
 * edge values for resizing
 *
 * These values are used to indicate which edge of a surface
 * is being dragged in a resize operation. The server may
 * use this information to adapt its behavior, e.g. choose
 * an appropriate cursor image.
 */
enum wl_shell_surface_resize {
	/**
	 * no edge
	 */
	WL_SHELL_SURFACE_RESIZE_NONE = 0,
	/**
	 * top edge
	 */
	WL_SHELL_SURFACE_RESIZE_TOP = 1,
	/**
	 * bottom edge
	 */
	WL_SHELL_SURFACE_RESIZE_BOTTOM = 2,
	/**
	 * left edge
	 */
	WL_SHELL_SURFACE_RESIZE_LEFT = 4,
	/**
	 * top and left edges
	 */
	WL_SHELL_SURFACE_RESIZE_TOP_LEFT = 5,
	/**
	 * bottom and left edges
	 */
	WL_SHELL_SURFACE_RESIZE_BOTTOM_LEFT = 6,
	/**
	 * right edge
	 */
	WL_SHELL_SURFACE_RESIZE_RIGHT = 8,
	/**
	 * top and right edges
	 */
	WL_SHELL_SURFACE_RESIZE_TOP_RIGHT = 9,
	/**
	 * bottom and right edges
	 */
	WL_SHELL_SURFACE_RESIZE_BOTTOM_RIGHT = 10,
};
#endif /* WL_SHELL_SURFACE_RESIZE_ENUM */

#ifndef WL_SHELL_SURFACE_TRANSIENT_ENUM
#define WL_SHELL_SURFACE_TRANSIENT_ENUM
/**
 * @ingroup iface_wl_shell_surface
 * details of transient behaviour
 *
 * These flags specify details of the expected behaviour
 * of transient surfaces. Used in the set_transient request.
 */
enum wl_shell_surface_transient {
	/**
	 * do not set keyboard focus
	 */
	WL_SHELL_SURFACE_TRANSIENT_INACTIVE = 0x1,
};
#endif /* WL_SHELL_SURFACE_TRANSIENT_ENUM */

#ifndef WL_SHELL_SURFACE_FULLSCREEN_METHOD_ENUM
#define WL_SHELL_SURFACE_FULLSCREEN_METHOD_ENUM
/**
 * @ingroup iface_wl_shell_surface
 * different method to set the surface fullscreen
 *
 * Hints to indicate to the compositor how to deal with a conflict
 * between the dimensions of the surface and the dimensions of the
 * output. The compositor is free to ignore this parameter.
 */
enum wl_shell_surface_fullscreen_method {
	/**
	 * no preference, apply default policy
	 */
	WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT = 0,
	/**
	 * scale, preserve the surface's aspect ratio and center on output
	 */
	WL_SHELL_SURFACE_FULLSCREEN_METHOD_SCALE = 1,
	/**
	 * switch output mode to the smallest mode that can fit the surface, add black borders to compensate size mismatch
	 */
	WL_SHELL_SURFACE_FULLSCREEN_METHOD_DRIVER = 2,
	/**
	 * no upscaling, center on output and add black borders to compensate size mismatch
	 */
	WL_SHELL_SURFACE_FULLSCREEN_METHOD_FILL = 3,
};
#endif /* WL_SHELL_SURFACE_FULLSCREEN_METHOD_ENUM */

/**
 * @ingroup iface_wl_shell_surface
 * @struct wl_shell_surface_listener
 */
struct wl_shell_surface_listener {
	/**
	 * ping client
	 *
	 * Ping a client to check if it is receiving events and sending
	 * requests. A client is expected to reply with a pong request.
	 * @param serial serial number of the ping
	 */
	void (*ping)(void *data,
		     struct wl_shell_surface *wl_shell_surface,
		     uint32_t serial);
	/**
	 * suggest resize
	 *
	 * The configure event asks the client to resize its surface.
	 *
	 * The size is a hint, in the sense that the client is free to
	 * ignore it if it doesn't resize, pick a smaller size (to satisfy
	 * aspect ratio or resize in steps of NxM pixels).
	 *
	 * The edges parameter provides a hint about how the surface was
	 * resized. The client may use this information to decide how to
	 * adjust its content to the new size (e.g. a scrolling area might
	 * adjust its content position to leave the viewable content
	 * unmoved).
	 *
	 * The client is free to dismiss all but the last configure event
	 * it received.
	 *
	 * The width and height arguments specify the size of the window in
	 * surface-local coordinates.
	 * @param edges how the surface was resized
	 * @param width new width of the surface
	 * @param height new height of the surface
	 */
	void (*configure)(void *data,
			  struct wl_shell_surface *wl_shell_surface,
			  uint32_t edges,
			  int32_t width,
			  int32_t height);
	/**
	 * popup interaction is done
	 *
	 * The popup_done event is sent out when a popup grab is broken,
	 * that is, when the user clicks a surface that doesn't belong to
	 * the client owning the popup surface.
	 */
	void (*popup_done)(void *data,
			   struct wl_shell_surface *wl_shell_surface);
};

/**
 * @ingroup iface_wl_shell_surface
 */
static inline int
wl_shell_surface_add_listener(struct wl_shell_surface *wl_shell_surface,
			      const struct wl_shell_surface_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_shell_surface,
				     (void (**)(void)) listener, data);
}

#define WL_SHELL_SURFACE_PONG 0
#define WL_SHELL_SURFACE_MOVE 1
#define WL_SHELL_SURFACE_RESIZE 2
#define WL_SHELL_SURFACE_SET_TOPLEVEL 3
#define WL_SHELL_SURFACE_SET_TRANSIENT 4
#define WL_SHELL_SURFACE_SET_FULLSCREEN 5
#define WL_SHELL_SURFACE_SET_POPUP 6
#define WL_SHELL_SURFACE_SET_MAXIMIZED 7
#define WL_SHELL_SURFACE_SET_TITLE 8
#define WL_SHELL_SURFACE_SET_CLASS 9

/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_PING_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_CONFIGURE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_POPUP_DONE_SINCE_VERSION 1

/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_PONG_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_MOVE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_RESIZE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_SET_TOPLEVEL_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_SET_TRANSIENT_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_SET_FULLSCREEN_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_SET_POPUP_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_SET_MAXIMIZED_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_SET_TITLE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_shell_surface
 */
#define WL_SHELL_SURFACE_SET_CLASS_SINCE_VERSION 1

/** @ingroup iface_wl_shell_surface */
static inline void
wl_shell_surface_set_user_data(struct wl_shell_surface *wl_shell_surface, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_shell_surface, user_data);
}

/** @ingroup iface_wl_shell_surface */
static inline void *
wl_shell_surface_get_user_data(struct wl_shell_surface *wl_shell_surface)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_shell_surface);
}

static inline uint32_t
wl_shell_surface_get_version(struct wl_shell_surface *wl_shell_surface)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_shell_surface);
}

/** @ingroup iface_wl_shell_surface */
static inline void
wl_shell_surface_destroy(struct wl_shell_surface *wl_shell_surface)
{
	wl_proxy_destroy((struct wl_proxy *) wl_shell_surface);
}

/**
 * @ingroup iface_wl_shell_surface
 *
 * A client must respond to a ping event with a pong request or
 * the client may be deemed unresponsive.
 */
static inline void
wl_shell_surface_pong(struct wl_shell_surface *wl_shell_surface, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_PONG, serial);
}

/**
 * @ingroup iface_wl_shell_surface
 *
 * Start a pointer-driven move of the surface.
 *
 * This request must be used in response to a button press event.
 * The server may ignore move requests depending on the state of
 * the surface (e.g. fullscreen or maximized).
 */
static inline void
wl_shell_surface_move(struct wl_shell_surface *wl_shell_surface, struct wl_seat *seat, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_MOVE, seat, serial);
}

/**
 * @ingroup iface_wl_shell_surface
 *
 * Start a pointer-driven resizing of the surface.
 *
 * This request must be used in response to a button press event.
 * The server may ignore resize requests depending on the state of
 * the surface (e.g. fullscreen or maximized).
 */
static inline void
wl_shell_surface_resize(struct wl_shell_surface *wl_shell_surface, struct wl_seat *seat, uint32_t serial, uint32_t edges)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_RESIZE, seat, serial, edges);
}

/**
 * @ingroup iface_wl_shell_surface
 *
 * Map the surface as a toplevel surface.
 *
 * A toplevel surface is not fullscreen, maximized or transient.
 */
static inline void
wl_shell_surface_set_toplevel(struct wl_shell_surface *wl_shell_surface)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_TOPLEVEL);
}

/**
 * @ingroup iface_wl_shell_surface
 *
 * Map the surface relative to an existing surface.
 *
 * The x and y arguments specify the location of the upper left
 * corner of the surface relative to the upper left corner of the
 * parent surface, in surface-local coordinates.
 *
 * The flags argument controls details of the transient behaviour.
 */
static inline void
wl_shell_surface_set_transient(struct wl_shell_surface *wl_shell_surface, struct wl_surface *parent, int32_t x, int32_t y, uint32_t flags)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_TRANSIENT, parent, x, y, flags);
}

/**
 * @ingroup iface_wl_shell_surface
 *
 * Map the surface as a fullscreen surface.
 *
 * If an output parameter is given then the surface will be made
 * fullscreen on that output. If the client does not specify the
 * output then the compositor will apply its policy - usually
 * choosing the output on which the surface has the biggest surface
 * area.
 *
 * The client may specify a method to resolve a size conflict
 * between the output size and the surface size - this is provided
 * through the method parameter.
 *
 * The framerate parameter is used only when the method is set
 * to "driver", to indicate the preferred framerate. A value of 0
 * indicates that the client does not care about framerate.  The
 * framerate is specified in mHz, that is framerate of 60000 is 60Hz.
 *
 * A method of "scale" or "driver" implies a scaling operation of
 * the surface, either via a direct scaling operation or a change of
 * the output mode. This will override any kind of output scaling, so
 * that mapping a surface with a buffer size equal to the mode can
 * fill the screen independent of buffer_scale.
 *
 * A method of "fill" means we don't scale up the buffer, however
 * any output scale is applied. This means that you may run into
 * an edge case where the application maps a buffer with the same
 * size of the output mode but buffer_scale 1 (thus making a
 * surface larger than the output). In this case it is allowed to
 * downscale the results to fit the screen.
 *
 * The compositor must reply to this request with a configure event
 * with the dimensions for the output on which the surface will
 * be made fullscreen.
 */
static inline void
wl_shell_surface_set_fullscreen(struct wl_shell_surface *wl_shell_surface, uint32_t method, uint32_t framerate, struct wl_output *output)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_FULLSCREEN, method, framerate, output);
}

/**
 * @ingroup iface_wl_shell_surface
 *
 * Map the surface as a popup.
 *
 * A popup surface is a transient surface with an added pointer
 * grab.
 *
 * An existing implicit grab will be changed to owner-events mode,
 * and the popup grab will continue after the implicit grab ends
 * (i.e. releasing the mouse button does not cause the popup to
 * be unmapped).
 *
 * The popup grab continues until the window is destroyed or a
 * mouse button is pressed in any other client's window. A click
 * in any of the client's surfaces is reported as normal, however,
 * clicks in other clients' surfaces will be discarded and trigger
 * the callback.
 *
 * The x and y arguments specify the location of the upper left
 * corner of the surface relative to the upper left corner of the
 * parent surface, in surface-local coordinates.
 */
static inline void
wl_shell_surface_set_popup(struct wl_shell_surface *wl_shell_surface, struct wl_seat *seat, uint32_t serial, struct wl_surface *parent, int32_t x, int32_t y, uint32_t flags)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_POPUP, seat, serial, parent, x, y, flags);
}

/**
 * @ingroup iface_wl_shell_surface
 *
 * Map the surface as a maximized surface.
 *
 * If an output parameter is given then the surface will be
 * maximized on that output. If the client does not specify the
 * output then the compositor will apply its policy - usually
 * choosing the output on which the surface has the biggest surface
 * area.
 *
 * The compositor will reply with a configure event telling
 * the expected new surface size. The operation is completed
 * on the next buffer attach to this surface.
 *
 * A maximized surface typically fills the entire output it is
 * bound to, except for desktop elements such as panels. This is
 * the main difference between a maximized shell surface and a
 * fullscreen shell surface.
 *
 * The details depend on the compositor implementation.
 */
static inline void
wl_shell_surface_set_maximized(struct wl_shell_surface *wl_shell_surface, struct wl_output *output)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_MAXIMIZED, output);
}

/**
 * @ingroup iface_wl_shell_surface
 *
 * Set a short title for the surface.
 *
 * This string may be used to identify the surface in a task bar,
 * window list, or other user interface elements provided by the
 * compositor.
 *
 * The string must be encoded in UTF-8.
 */
static inline void
wl_shell_surface_set_title(struct wl_shell_surface *wl_shell_surface, const char *title)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_TITLE, title);
}

/**
 * @ingroup iface_wl_shell_surface
 *
 * Set a class for the surface.
 *
 * The surface class identifies the general class of applications
 * to which the surface belongs. A common convention is to use the
 * file name (or the full path if it is a non-standard location) of
 * the application's .desktop file as the class.
 */
static inline void
wl_shell_surface_set_class(struct wl_shell_surface *wl_shell_surface, const char *class_)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_CLASS, class_);
}

#ifndef WL_SURFACE_ERROR_ENUM
#define WL_SURFACE_ERROR_ENUM
/**
 * @ingroup iface_wl_surface
 * wl_surface error values
 *
 * These errors can be emitted in response to wl_surface requests.
 */
enum wl_surface_error {
	/**
	 * buffer scale value is invalid
	 */
	WL_SURFACE_ERROR_INVALID_SCALE = 0,
	/**
	 * buffer transform value is invalid
	 */
	WL_SURFACE_ERROR_INVALID_TRANSFORM = 1,
};
#endif /* WL_SURFACE_ERROR_ENUM */

/**
 * @ingroup iface_wl_surface
 * @struct wl_surface_listener
 */
struct wl_surface_listener {
	/**
	 * surface enters an output
	 *
	 * This is emitted whenever a surface's creation, movement, or
	 * resizing results in some part of it being within the scanout
	 * region of an output.
	 *
	 * Note that a surface may be overlapping with zero or more
	 * outputs.
	 * @param output output entered by the surface
	 */
	void (*enter)(void *data,
		      struct wl_surface *wl_surface,
		      struct wl_output *output);
	/**
	 * surface leaves an output
	 *
	 * This is emitted whenever a surface's creation, movement, or
	 * resizing results in it no longer having any part of it within
	 * the scanout region of an output.
	 * @param output output left by the surface
	 */
	void (*leave)(void *data,
		      struct wl_surface *wl_surface,
		      struct wl_output *output);
};

/**
 * @ingroup iface_wl_surface
 */
static inline int
wl_surface_add_listener(struct wl_surface *wl_surface,
			const struct wl_surface_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_surface,
				     (void (**)(void)) listener, data);
}

#define WL_SURFACE_DESTROY 0
#define WL_SURFACE_ATTACH 1
#define WL_SURFACE_DAMAGE 2
#define WL_SURFACE_FRAME 3
#define WL_SURFACE_SET_OPAQUE_REGION 4
#define WL_SURFACE_SET_INPUT_REGION 5
#define WL_SURFACE_COMMIT 6
#define WL_SURFACE_SET_BUFFER_TRANSFORM 7
#define WL_SURFACE_SET_BUFFER_SCALE 8
#define WL_SURFACE_DAMAGE_BUFFER 9

/**
 * @ingroup iface_wl_surface
 */
#define WL_SURFACE_ENTER_SINCE_VERSION 1
/**
 * @ingroup iface_wl_surface
 */
#define WL_SURFACE_LEAVE_SINCE_VERSION 1

/**
 * @ingroup iface_wl_surface
 */
#define WL_SURFACE_DESTROY_SINCE_VERSION 1
/**
 * @ingroup iface_wl_surface
 */
#define WL_SURFACE_ATTACH_SINCE_VERSION 1
/**
 * @ingroup iface_wl_surface
 */
#define WL_SURFACE_DAMAGE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_surface
 */
#define WL_SURFACE_FRAME_SINCE_VERSION 1
/**
 * @ingroup iface_wl_surface
 */
#define WL_SURFACE_SET_OPAQUE_REGION_SINCE_VERSION 1
/**
 * @ingroup iface_wl_surface
 */
#define WL_SURFACE_SET_INPUT_REGION_SINCE_VERSION 1
/**
 * @ingroup iface_wl_surface
 */
#define WL_SURFACE_COMMIT_SINCE_VERSION 1
/**
 * @ingroup iface_wl_surface
 */
#define WL_SURFACE_SET_BUFFER_TRANSFORM_SINCE_VERSION 2
/**
 * @ingroup iface_wl_surface
 */
#define WL_SURFACE_SET_BUFFER_SCALE_SINCE_VERSION 3
/**
 * @ingroup iface_wl_surface
 */
#define WL_SURFACE_DAMAGE_BUFFER_SINCE_VERSION 4

/** @ingroup iface_wl_surface */
static inline void
wl_surface_set_user_data(struct wl_surface *wl_surface, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_surface, user_data);
}

/** @ingroup iface_wl_surface */
static inline void *
wl_surface_get_user_data(struct wl_surface *wl_surface)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_surface);
}

static inline uint32_t
wl_surface_get_version(struct wl_surface *wl_surface)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_surface);
}

/**
 * @ingroup iface_wl_surface
 *
 * Deletes the surface and invalidates its object ID.
 */
static inline void
wl_surface_destroy(struct wl_surface *wl_surface)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_surface);
}

/**
 * @ingroup iface_wl_surface
 *
 * Set a buffer as the content of this surface.
 *
 * The new size of the surface is calculated based on the buffer
 * size transformed by the inverse buffer_transform and the
 * inverse buffer_scale. This means that the supplied buffer
 * must be an integer multiple of the buffer_scale.
 *
 * The x and y arguments specify the location of the new pending
 * buffer's upper left corner, relative to the current buffer's upper
 * left corner, in surface-local coordinates. In other words, the
 * x and y, combined with the new surface size define in which
 * directions the surface's size changes.
 *
 * Surface contents are double-buffered state, see wl_surface.commit.
 *
 * The initial surface contents are void; there is no content.
 * wl_surface.attach assigns the given wl_buffer as the pending
 * wl_buffer. wl_surface.commit makes the pending wl_buffer the new
 * surface contents, and the size of the surface becomes the size
 * calculated from the wl_buffer, as described above. After commit,
 * there is no pending buffer until the next attach.
 *
 * Committing a pending wl_buffer allows the compositor to read the
 * pixels in the wl_buffer. The compositor may access the pixels at
 * any time after the wl_surface.commit request. When the compositor
 * will not access the pixels anymore, it will send the
 * wl_buffer.release event. Only after receiving wl_buffer.release,
 * the client may reuse the wl_buffer. A wl_buffer that has been
 * attached and then replaced by another attach instead of committed
 * will not receive a release event, and is not used by the
 * compositor.
 *
 * Destroying the wl_buffer after wl_buffer.release does not change
 * the surface contents. However, if the client destroys the
 * wl_buffer before receiving the wl_buffer.release event, the surface
 * contents become undefined immediately.
 *
 * If wl_surface.attach is sent with a NULL wl_buffer, the
 * following wl_surface.commit will remove the surface content.
 */
static inline void
wl_surface_attach(struct wl_surface *wl_surface, struct wl_buffer *buffer, int32_t x, int32_t y)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_ATTACH, buffer, x, y);
}

/**
 * @ingroup iface_wl_surface
 *
 * This request is used to describe the regions where the pending
 * buffer is different from the current surface contents, and where
 * the surface therefore needs to be repainted. The compositor
 * ignores the parts of the damage that fall outside of the surface.
 *
 * Damage is double-buffered state, see wl_surface.commit.
 *
 * The damage rectangle is specified in surface-local coordinates,
 * where x and y specify the upper left corner of the damage rectangle.
 *
 * The initial value for pending damage is empty: no damage.
 * wl_surface.damage adds pending damage: the new pending damage
 * is the union of old pending damage and the given rectangle.
 *
 * wl_surface.commit assigns pending damage as the current damage,
 * and clears pending damage. The server will clear the current
 * damage as it repaints the surface.
 *
 * Alternatively, damage can be posted with wl_surface.damage_buffer
 * which uses buffer coordinates instead of surface coordinates,
 * and is probably the preferred and intuitive way of doing this.
 */
static inline void
wl_surface_damage(struct wl_surface *wl_surface, int32_t x, int32_t y, int32_t width, int32_t height)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_DAMAGE, x, y, width, height);
}

/**
 * @ingroup iface_wl_surface
 *
 * Request a notification when it is a good time to start drawing a new
 * frame, by creating a frame callback. This is useful for throttling
 * redrawing operations, and driving animations.
 *
 * When a client is animating on a wl_surface, it can use the 'frame'
 * request to get notified when it is a good time to draw and commit the
 * next frame of animation. If the client commits an update earlier than
 * that, it is likely that some updates will not make it to the display,
 * and the client is wasting resources by drawing too often.
 *
 * The frame request will take effect on the next wl_surface.commit.
 * The notification will only be posted for one frame unless
 * requested again. For a wl_surface, the notifications are posted in
 * the order the frame requests were committed.
 *
 * The server must send the notifications so that a client
 * will not send excessive updates, while still allowing
 * the highest possible update rate for clients that wait for the reply
 * before drawing again. The server should give some time for the client
 * to draw and commit after sending the frame callback events to let it
 * hit the next output refresh.
 *
 * A server should avoid signaling the frame callbacks if the
 * surface is not visible in any way, e.g. the surface is off-screen,
 * or completely obscured by other opaque surfaces.
 *
 * The object returned by this request will be destroyed by the
 * compositor after the callback is fired and as such the client must not
 * attempt to use it after that point.
 *
 * The callback_data passed in the callback is the current time, in
 * milliseconds, with an undefined base.
 */
static inline struct wl_callback *
wl_surface_frame(struct wl_surface *wl_surface)
{
	struct wl_proxy *callback;

	callback = wl_proxy_marshal_constructor((struct wl_proxy *) wl_surface,
			 WL_SURFACE_FRAME, &wl_callback_interface, NULL);

	return (struct wl_callback *) callback;
}

/**
 * @ingroup iface_wl_surface
 *
 * This request sets the region of the surface that contains
 * opaque content.
 *
 * The opaque region is an optimization hint for the compositor
 * that lets it optimize the redrawing of content behind opaque
 * regions.  Setting an opaque region is not required for correct
 * behaviour, but marking transparent content as opaque will result
 * in repaint artifacts.
 *
 * The opaque region is specified in surface-local coordinates.
 *
 * The compositor ignores the parts of the opaque region that fall
 * outside of the surface.
 *
 * Opaque region is double-buffered state, see wl_surface.commit.
 *
 * wl_surface.set_opaque_region changes the pending opaque region.
 * wl_surface.commit copies the pending region to the current region.
 * Otherwise, the pending and current regions are never changed.
 *
 * The initial value for an opaque region is empty. Setting the pending
 * opaque region has copy semantics, and the wl_region object can be
 * destroyed immediately. A NULL wl_region causes the pending opaque
 * region to be set to empty.
 */
static inline void
wl_surface_set_opaque_region(struct wl_surface *wl_surface, struct wl_region *region)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_SET_OPAQUE_REGION, region);
}

/**
 * @ingroup iface_wl_surface
 *
 * This request sets the region of the surface that can receive
 * pointer and touch events.
 *
 * Input events happening outside of this region will try the next
 * surface in the server surface stack. The compositor ignores the
 * parts of the input region that fall outside of the surface.
 *
 * The input region is specified in surface-local coordinates.
 *
 * Input region is double-buffered state, see wl_surface.commit.
 *
 * wl_surface.set_input_region changes the pending input region.
 * wl_surface.commit copies the pending region to the current region.
 * Otherwise the pending and current regions are never changed,
 * except cursor and icon surfaces are special cases, see
 * wl_pointer.set_cursor and wl_data_device.start_drag.
 *
 * The initial value for an input region is infinite. That means the
 * whole surface will accept input. Setting the pending input region
 * has copy semantics, and the wl_region object can be destroyed
 * immediately. A NULL wl_region causes the input region to be set
 * to infinite.
 */
static inline void
wl_surface_set_input_region(struct wl_surface *wl_surface, struct wl_region *region)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_SET_INPUT_REGION, region);
}

/**
 * @ingroup iface_wl_surface
 *
 * Surface state (input, opaque, and damage regions, attached buffers,
 * etc.) is double-buffered. Protocol requests modify the pending state,
 * as opposed to the current state in use by the compositor. A commit
 * request atomically applies all pending state, replacing the current
 * state. After commit, the new pending state is as documented for each
 * related request.
 *
 * On commit, a pending wl_buffer is applied first, and all other state
 * second. This means that all coordinates in double-buffered state are
 * relative to the new wl_buffer coming into use, except for
 * wl_surface.attach itself. If there is no pending wl_buffer, the
 * coordinates are relative to the current surface contents.
 *
 * All requests that need a commit to become effective are documented
 * to affect double-buffered state.
 *
 * Other interfaces may add further double-buffered surface state.
 */
static inline void
wl_surface_commit(struct wl_surface *wl_surface)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_COMMIT);
}

/**
 * @ingroup iface_wl_surface
 *
 * This request sets an optional transformation on how the compositor
 * interprets the contents of the buffer attached to the surface. The
 * accepted values for the transform parameter are the values for
 * wl_output.transform.
 *
 * Buffer transform is double-buffered state, see wl_surface.commit.
 *
 * A newly created surface has its buffer transformation set to normal.
 *
 * wl_surface.set_buffer_transform changes the pending buffer
 * transformation. wl_surface.commit copies the pending buffer
 * transformation to the current one. Otherwise, the pending and current
 * values are never changed.
 *
 * The purpose of this request is to allow clients to render content
 * according to the output transform, thus permitting the compositor to
 * use certain optimizations even if the display is rotated. Using
 * hardware overlays and scanning out a client buffer for fullscreen
 * surfaces are examples of such optimizations. Those optimizations are
 * highly dependent on the compositor implementation, so the use of this
 * request should be considered on a case-by-case basis.
 *
 * Note that if the transform value includes 90 or 270 degree rotation,
 * the width of the buffer will become the surface height and the height
 * of the buffer will become the surface width.
 *
 * If transform is not one of the values from the
 * wl_output.transform enum the invalid_transform protocol error
 * is raised.
 */
static inline void
wl_surface_set_buffer_transform(struct wl_surface *wl_surface, int32_t transform)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_SET_BUFFER_TRANSFORM, transform);
}

/**
 * @ingroup iface_wl_surface
 *
 * This request sets an optional scaling factor on how the compositor
 * interprets the contents of the buffer attached to the window.
 *
 * Buffer scale is double-buffered state, see wl_surface.commit.
 *
 * A newly created surface has its buffer scale set to 1.
 *
 * wl_surface.set_buffer_scale changes the pending buffer scale.
 * wl_surface.commit copies the pending buffer scale to the current one.
 * Otherwise, the pending and current values are never changed.
 *
 * The purpose of this request is to allow clients to supply higher
 * resolution buffer data for use on high resolution outputs. It is
 * intended that you pick the same buffer scale as the scale of the
 * output that the surface is displayed on. This means the compositor
 * can avoid scaling when rendering the surface on that output.
 *
 * Note that if the scale is larger than 1, then you have to attach
 * a buffer that is larger (by a factor of scale in each dimension)
 * than the desired surface size.
 *
 * If scale is not positive the invalid_scale protocol error is
 * raised.
 */
static inline void
wl_surface_set_buffer_scale(struct wl_surface *wl_surface, int32_t scale)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_SET_BUFFER_SCALE, scale);
}

/**
 * @ingroup iface_wl_surface
 *
 * This request is used to describe the regions where the pending
 * buffer is different from the current surface contents, and where
 * the surface therefore needs to be repainted. The compositor
 * ignores the parts of the damage that fall outside of the surface.
 *
 * Damage is double-buffered state, see wl_surface.commit.
 *
 * The damage rectangle is specified in buffer coordinates,
 * where x and y specify the upper left corner of the damage rectangle.
 *
 * The initial value for pending damage is empty: no damage.
 * wl_surface.damage_buffer adds pending damage: the new pending
 * damage is the union of old pending damage and the given rectangle.
 *
 * wl_surface.commit assigns pending damage as the current damage,
 * and clears pending damage. The server will clear the current
 * damage as it repaints the surface.
 *
 * This request differs from wl_surface.damage in only one way - it
 * takes damage in buffer coordinates instead of surface-local
 * coordinates. While this generally is more intuitive than surface
 * coordinates, it is especially desirable when using wp_viewport
 * or when a drawing library (like EGL) is unaware of buffer scale
 * and buffer transform.
 *
 * Note: Because buffer transformation changes and damage requests may
 * be interleaved in the protocol stream, it is impossible to determine
 * the actual mapping between surface and buffer damage until
 * wl_surface.commit time. Therefore, compositors wishing to take both
 * kinds of damage into account will have to accumulate damage from the
 * two requests separately and only transform from one to the other
 * after receiving the wl_surface.commit.
 */
static inline void
wl_surface_damage_buffer(struct wl_surface *wl_surface, int32_t x, int32_t y, int32_t width, int32_t height)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_DAMAGE_BUFFER, x, y, width, height);
}

#ifndef WL_SEAT_CAPABILITY_ENUM
#define WL_SEAT_CAPABILITY_ENUM
/**
 * @ingroup iface_wl_seat
 * seat capability bitmask
 *
 * This is a bitmask of capabilities this seat has; if a member is
 * set, then it is present on the seat.
 */
enum wl_seat_capability {
	/**
	 * the seat has pointer devices
	 */
	WL_SEAT_CAPABILITY_POINTER = 1,
	/**
	 * the seat has one or more keyboards
	 */
	WL_SEAT_CAPABILITY_KEYBOARD = 2,
	/**
	 * the seat has touch devices
	 */
	WL_SEAT_CAPABILITY_TOUCH = 4,
};
#endif /* WL_SEAT_CAPABILITY_ENUM */

/**
 * @ingroup iface_wl_seat
 * @struct wl_seat_listener
 */
struct wl_seat_listener {
	/**
	 * seat capabilities changed
	 *
	 * This is emitted whenever a seat gains or loses the pointer,
	 * keyboard or touch capabilities. The argument is a capability
	 * enum containing the complete set of capabilities this seat has.
	 *
	 * When the pointer capability is added, a client may create a
	 * wl_pointer object using the wl_seat.get_pointer request. This
	 * object will receive pointer events until the capability is
	 * removed in the future.
	 *
	 * When the pointer capability is removed, a client should destroy
	 * the wl_pointer objects associated with the seat where the
	 * capability was removed, using the wl_pointer.release request. No
	 * further pointer events will be received on these objects.
	 *
	 * In some compositors, if a seat regains the pointer capability
	 * and a client has a previously obtained wl_pointer object of
	 * version 4 or less, that object may start sending pointer events
	 * again. This behavior is considered a misinterpretation of the
	 * intended behavior and must not be relied upon by the client.
	 * wl_pointer objects of version 5 or later must not send events if
	 * created before the most recent event notifying the client of an
	 * added pointer capability.
	 *
	 * The above behavior also applies to wl_keyboard and wl_touch with
	 * the keyboard and touch capabilities, respectively.
	 * @param capabilities capabilities of the seat
	 */
	void (*capabilities)(void *data,
			     struct wl_seat *wl_seat,
			     uint32_t capabilities);
	/**
	 * unique identifier for this seat
	 *
	 * In a multiseat configuration this can be used by the client to
	 * help identify which physical devices the seat represents. Based
	 * on the seat configuration used by the compositor.
	 * @param name seat identifier
	 * @since 2
	 */
	void (*name)(void *data,
		     struct wl_seat *wl_seat,
		     const char *name);
};

/**
 * @ingroup iface_wl_seat
 */
static inline int
wl_seat_add_listener(struct wl_seat *wl_seat,
		     const struct wl_seat_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_seat,
				     (void (**)(void)) listener, data);
}

#define WL_SEAT_GET_POINTER 0
#define WL_SEAT_GET_KEYBOARD 1
#define WL_SEAT_GET_TOUCH 2
#define WL_SEAT_RELEASE 3

/**
 * @ingroup iface_wl_seat
 */
#define WL_SEAT_CAPABILITIES_SINCE_VERSION 1
/**
 * @ingroup iface_wl_seat
 */
#define WL_SEAT_NAME_SINCE_VERSION 2

/**
 * @ingroup iface_wl_seat
 */
#define WL_SEAT_GET_POINTER_SINCE_VERSION 1
/**
 * @ingroup iface_wl_seat
 */
#define WL_SEAT_GET_KEYBOARD_SINCE_VERSION 1
/**
 * @ingroup iface_wl_seat
 */
#define WL_SEAT_GET_TOUCH_SINCE_VERSION 1
/**
 * @ingroup iface_wl_seat
 */
#define WL_SEAT_RELEASE_SINCE_VERSION 5

/** @ingroup iface_wl_seat */
static inline void
wl_seat_set_user_data(struct wl_seat *wl_seat, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_seat, user_data);
}

/** @ingroup iface_wl_seat */
static inline void *
wl_seat_get_user_data(struct wl_seat *wl_seat)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_seat);
}

static inline uint32_t
wl_seat_get_version(struct wl_seat *wl_seat)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_seat);
}

/** @ingroup iface_wl_seat */
static inline void
wl_seat_destroy(struct wl_seat *wl_seat)
{
	wl_proxy_destroy((struct wl_proxy *) wl_seat);
}

/**
 * @ingroup iface_wl_seat
 *
 * The ID provided will be initialized to the wl_pointer interface
 * for this seat.
 *
 * This request only takes effect if the seat has the pointer
 * capability, or has had the pointer capability in the past.
 * It is a protocol violation to issue this request on a seat that has
 * never had the pointer capability.
 */
static inline struct wl_pointer *
wl_seat_get_pointer(struct wl_seat *wl_seat)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) wl_seat,
			 WL_SEAT_GET_POINTER, &wl_pointer_interface, NULL);

	return (struct wl_pointer *) id;
}

/**
 * @ingroup iface_wl_seat
 *
 * The ID provided will be initialized to the wl_keyboard interface
 * for this seat.
 *
 * This request only takes effect if the seat has the keyboard
 * capability, or has had the keyboard capability in the past.
 * It is a protocol violation to issue this request on a seat that has
 * never had the keyboard capability.
 */
static inline struct wl_keyboard *
wl_seat_get_keyboard(struct wl_seat *wl_seat)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) wl_seat,
			 WL_SEAT_GET_KEYBOARD, &wl_keyboard_interface, NULL);

	return (struct wl_keyboard *) id;
}

/**
 * @ingroup iface_wl_seat
 *
 * The ID provided will be initialized to the wl_touch interface
 * for this seat.
 *
 * This request only takes effect if the seat has the touch
 * capability, or has had the touch capability in the past.
 * It is a protocol violation to issue this request on a seat that has
 * never had the touch capability.
 */
static inline struct wl_touch *
wl_seat_get_touch(struct wl_seat *wl_seat)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) wl_seat,
			 WL_SEAT_GET_TOUCH, &wl_touch_interface, NULL);

	return (struct wl_touch *) id;
}

/**
 * @ingroup iface_wl_seat
 *
 * Using this request a client can tell the server that it is not going to
 * use the seat object anymore.
 */
static inline void
wl_seat_release(struct wl_seat *wl_seat)
{
	wl_proxy_marshal((struct wl_proxy *) wl_seat,
			 WL_SEAT_RELEASE);

	wl_proxy_destroy((struct wl_proxy *) wl_seat);
}

#ifndef WL_POINTER_ERROR_ENUM
#define WL_POINTER_ERROR_ENUM
enum wl_pointer_error {
	/**
	 * given wl_surface has another role
	 */
	WL_POINTER_ERROR_ROLE = 0,
};
#endif /* WL_POINTER_ERROR_ENUM */

#ifndef WL_POINTER_BUTTON_STATE_ENUM
#define WL_POINTER_BUTTON_STATE_ENUM
/**
 * @ingroup iface_wl_pointer
 * physical button state
 *
 * Describes the physical state of a button that produced the button
 * event.
 */
enum wl_pointer_button_state {
	/**
	 * the button is not pressed
	 */
	WL_POINTER_BUTTON_STATE_RELEASED = 0,
	/**
	 * the button is pressed
	 */
	WL_POINTER_BUTTON_STATE_PRESSED = 1,
};
#endif /* WL_POINTER_BUTTON_STATE_ENUM */

#ifndef WL_POINTER_AXIS_ENUM
#define WL_POINTER_AXIS_ENUM
/**
 * @ingroup iface_wl_pointer
 * axis types
 *
 * Describes the axis types of scroll events.
 */
enum wl_pointer_axis {
	/**
	 * vertical axis
	 */
	WL_POINTER_AXIS_VERTICAL_SCROLL = 0,
	/**
	 * horizontal axis
	 */
	WL_POINTER_AXIS_HORIZONTAL_SCROLL = 1,
};
#endif /* WL_POINTER_AXIS_ENUM */

#ifndef WL_POINTER_AXIS_SOURCE_ENUM
#define WL_POINTER_AXIS_SOURCE_ENUM
/**
 * @ingroup iface_wl_pointer
 * axis source types
 *
 * Describes the source types for axis events. This indicates to the
 * client how an axis event was physically generated; a client may
 * adjust the user interface accordingly. For example, scroll events
 * from a "finger" source may be in a smooth coordinate space with
 * kinetic scrolling whereas a "wheel" source may be in discrete steps
 * of a number of lines.
 *
 * The "continuous" axis source is a device generating events in a
 * continuous coordinate space, but using something other than a
 * finger. One example for this source is button-based scrolling where
 * the vertical motion of a device is converted to scroll events while
 * a button is held down.
 *
 * The "wheel tilt" axis source indicates that the actual device is a
 * wheel but the scroll event is not caused by a rotation but a
 * (usually sideways) tilt of the wheel.
 */
enum wl_pointer_axis_source {
	/**
	 * a physical wheel rotation
	 */
	WL_POINTER_AXIS_SOURCE_WHEEL = 0,
	/**
	 * finger on a touch surface
	 */
	WL_POINTER_AXIS_SOURCE_FINGER = 1,
	/**
	 * continuous coordinate space
	 */
	WL_POINTER_AXIS_SOURCE_CONTINUOUS = 2,
	/**
	 * a physical wheel tilt
	 * @since 6
	 */
	WL_POINTER_AXIS_SOURCE_WHEEL_TILT = 3,
};
/**
 * @ingroup iface_wl_pointer
 */
#define WL_POINTER_AXIS_SOURCE_WHEEL_TILT_SINCE_VERSION 6
#endif /* WL_POINTER_AXIS_SOURCE_ENUM */

/**
 * @ingroup iface_wl_pointer
 * @struct wl_pointer_listener
 */
struct wl_pointer_listener {
	/**
	 * enter event
	 *
	 * Notification that this seat's pointer is focused on a certain
	 * surface.
	 *
	 * When a seat's focus enters a surface, the pointer image is
	 * undefined and a client should respond to this event by setting
	 * an appropriate pointer image with the set_cursor request.
	 * @param serial serial number of the enter event
	 * @param surface surface entered by the pointer
	 * @param surface_x surface-local x coordinate
	 * @param surface_y surface-local y coordinate
	 */
	void (*enter)(void *data,
		      struct wl_pointer *wl_pointer,
		      uint32_t serial,
		      struct wl_surface *surface,
		      wl_fixed_t surface_x,
		      wl_fixed_t surface_y);
	/**
	 * leave event
	 *
	 * Notification that this seat's pointer is no longer focused on
	 * a certain surface.
	 *
	 * The leave notification is sent before the enter notification for
	 * the new focus.
	 * @param serial serial number of the leave event
	 * @param surface surface left by the pointer
	 */
	void (*leave)(void *data,
		      struct wl_pointer *wl_pointer,
		      uint32_t serial,
		      struct wl_surface *surface);
	/**
	 * pointer motion event
	 *
	 * Notification of pointer location change. The arguments
	 * surface_x and surface_y are the location relative to the focused
	 * surface.
	 * @param time timestamp with millisecond granularity
	 * @param surface_x surface-local x coordinate
	 * @param surface_y surface-local y coordinate
	 */
	void (*motion)(void *data,
		       struct wl_pointer *wl_pointer,
		       uint32_t time,
		       wl_fixed_t surface_x,
		       wl_fixed_t surface_y);
	/**
	 * pointer button event
	 *
	 * Mouse button click and release notifications.
	 *
	 * The location of the click is given by the last motion or enter
	 * event. The time argument is a timestamp with millisecond
	 * granularity, with an undefined base.
	 *
	 * The button is a button code as defined in the Linux kernel's
	 * linux/input-event-codes.h header file, e.g. BTN_LEFT.
	 *
	 * Any 16-bit button code value is reserved for future additions to
	 * the kernel's event code list. All other button codes above
	 * 0xFFFF are currently undefined but may be used in future
	 * versions of this protocol.
	 * @param serial serial number of the button event
	 * @param time timestamp with millisecond granularity
	 * @param button button that produced the event
	 * @param state physical state of the button
	 */
	void (*button)(void *data,
		       struct wl_pointer *wl_pointer,
		       uint32_t serial,
		       uint32_t time,
		       uint32_t button,
		       uint32_t state);
	/**
	 * axis event
	 *
	 * Scroll and other axis notifications.
	 *
	 * For scroll events (vertical and horizontal scroll axes), the
	 * value parameter is the length of a vector along the specified
	 * axis in a coordinate space identical to those of motion events,
	 * representing a relative movement along the specified axis.
	 *
	 * For devices that support movements non-parallel to axes multiple
	 * axis events will be emitted.
	 *
	 * When applicable, for example for touch pads, the server can
	 * choose to emit scroll events where the motion vector is
	 * equivalent to a motion event vector.
	 *
	 * When applicable, a client can transform its content relative to
	 * the scroll distance.
	 * @param time timestamp with millisecond granularity
	 * @param axis axis type
	 * @param value length of vector in surface-local coordinate space
	 */
	void (*axis)(void *data,
		     struct wl_pointer *wl_pointer,
		     uint32_t time,
		     uint32_t axis,
		     wl_fixed_t value);
	/**
	 * end of a pointer event sequence
	 *
	 * Indicates the end of a set of events that logically belong
	 * together. A client is expected to accumulate the data in all
	 * events within the frame before proceeding.
	 *
	 * All wl_pointer events before a wl_pointer.frame event belong
	 * logically together. For example, in a diagonal scroll motion the
	 * compositor will send an optional wl_pointer.axis_source event,
	 * two wl_pointer.axis events (horizontal and vertical) and finally
	 * a wl_pointer.frame event. The client may use this information to
	 * calculate a diagonal vector for scrolling.
	 *
	 * When multiple wl_pointer.axis events occur within the same
	 * frame, the motion vector is the combined motion of all events.
	 * When a wl_pointer.axis and a wl_pointer.axis_stop event occur
	 * within the same frame, this indicates that axis movement in one
	 * axis has stopped but continues in the other axis. When multiple
	 * wl_pointer.axis_stop events occur within the same frame, this
	 * indicates that these axes stopped in the same instance.
	 *
	 * A wl_pointer.frame event is sent for every logical event group,
	 * even if the group only contains a single wl_pointer event.
	 * Specifically, a client may get a sequence: motion, frame,
	 * button, frame, axis, frame, axis_stop, frame.
	 *
	 * The wl_pointer.enter and wl_pointer.leave events are logical
	 * events generated by the compositor and not the hardware. These
	 * events are also grouped by a wl_pointer.frame. When a pointer
	 * moves from one surface to another, a compositor should group the
	 * wl_pointer.leave event within the same wl_pointer.frame.
	 * However, a client must not rely on wl_pointer.leave and
	 * wl_pointer.enter being in the same wl_pointer.frame.
	 * Compositor-specific policies may require the wl_pointer.leave
	 * and wl_pointer.enter event being split across multiple
	 * wl_pointer.frame groups.
	 * @since 5
	 */
	void (*frame)(void *data,
		      struct wl_pointer *wl_pointer);
	/**
	 * axis source event
	 *
	 * Source information for scroll and other axes.
	 *
	 * This event does not occur on its own. It is sent before a
	 * wl_pointer.frame event and carries the source information for
	 * all events within that frame.
	 *
	 * The source specifies how this event was generated. If the source
	 * is wl_pointer.axis_source.finger, a wl_pointer.axis_stop event
	 * will be sent when the user lifts the finger off the device.
	 *
	 * If the source is wl_pointer.axis_source.wheel,
	 * wl_pointer.axis_source.wheel_tilt or
	 * wl_pointer.axis_source.continuous, a wl_pointer.axis_stop event
	 * may or may not be sent. Whether a compositor sends an axis_stop
	 * event for these sources is hardware-specific and
	 * implementation-dependent; clients must not rely on receiving an
	 * axis_stop event for these scroll sources and should treat scroll
	 * sequences from these scroll sources as unterminated by default.
	 *
	 * This event is optional. If the source is unknown for a
	 * particular axis event sequence, no event is sent. Only one
	 * wl_pointer.axis_source event is permitted per frame.
	 *
	 * The order of wl_pointer.axis_discrete and wl_pointer.axis_source
	 * is not guaranteed.
	 * @param axis_source source of the axis event
	 * @since 5
	 */
	void (*axis_source)(void *data,
			    struct wl_pointer *wl_pointer,
			    uint32_t axis_source);
	/**
	 * axis stop event
	 *
	 * Stop notification for scroll and other axes.
	 *
	 * For some wl_pointer.axis_source types, a wl_pointer.axis_stop
	 * event is sent to notify a client that the axis sequence has
	 * terminated. This enables the client to implement kinetic
	 * scrolling. See the wl_pointer.axis_source documentation for
	 * information on when this event may be generated.
	 *
	 * Any wl_pointer.axis events with the same axis_source after this
	 * event should be considered as the start of a new axis motion.
	 *
	 * The timestamp is to be interpreted identical to the timestamp in
	 * the wl_pointer.axis event. The timestamp value may be the same
	 * as a preceding wl_pointer.axis event.
	 * @param time timestamp with millisecond granularity
	 * @param axis the axis stopped with this event
	 * @since 5
	 */
	void (*axis_stop)(void *data,
			  struct wl_pointer *wl_pointer,
			  uint32_t time,
			  uint32_t axis);
	/**
	 * axis click event
	 *
	 * Discrete step information for scroll and other axes.
	 *
	 * This event carries the axis value of the wl_pointer.axis event
	 * in discrete steps (e.g. mouse wheel clicks).
	 *
	 * This event does not occur on its own, it is coupled with a
	 * wl_pointer.axis event that represents this axis value on a
	 * continuous scale. The protocol guarantees that each
	 * axis_discrete event is always followed by exactly one axis event
	 * with the same axis number within the same wl_pointer.frame. Note
	 * that the protocol allows for other events to occur between the
	 * axis_discrete and its coupled axis event, including other
	 * axis_discrete or axis events.
	 *
	 * This event is optional; continuous scrolling devices like
	 * two-finger scrolling on touchpads do not have discrete steps and
	 * do not generate this event.
	 *
	 * The discrete value carries the directional information. e.g. a
	 * value of -2 is two steps towards the negative direction of this
	 * axis.
	 *
	 * The axis number is identical to the axis number in the
	 * associated axis event.
	 *
	 * The order of wl_pointer.axis_discrete and wl_pointer.axis_source
	 * is not guaranteed.
	 * @param axis axis type
	 * @param discrete number of steps
	 * @since 5
	 */
	void (*axis_discrete)(void *data,
			      struct wl_pointer *wl_pointer,
			      uint32_t axis,
			      int32_t discrete);
};

/**
 * @ingroup iface_wl_pointer
 */
static inline int
wl_pointer_add_listener(struct wl_pointer *wl_pointer,
			const struct wl_pointer_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_pointer,
				     (void (**)(void)) listener, data);
}

#define WL_POINTER_SET_CURSOR 0
#define WL_POINTER_RELEASE 1

/**
 * @ingroup iface_wl_pointer
 */
#define WL_POINTER_ENTER_SINCE_VERSION 1
/**
 * @ingroup iface_wl_pointer
 */
#define WL_POINTER_LEAVE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_pointer
 */
#define WL_POINTER_MOTION_SINCE_VERSION 1
/**
 * @ingroup iface_wl_pointer
 */
#define WL_POINTER_BUTTON_SINCE_VERSION 1
/**
 * @ingroup iface_wl_pointer
 */
#define WL_POINTER_AXIS_SINCE_VERSION 1
/**
 * @ingroup iface_wl_pointer
 */
#define WL_POINTER_FRAME_SINCE_VERSION 5
/**
 * @ingroup iface_wl_pointer
 */
#define WL_POINTER_AXIS_SOURCE_SINCE_VERSION 5
/**
 * @ingroup iface_wl_pointer
 */
#define WL_POINTER_AXIS_STOP_SINCE_VERSION 5
/**
 * @ingroup iface_wl_pointer
 */
#define WL_POINTER_AXIS_DISCRETE_SINCE_VERSION 5

/**
 * @ingroup iface_wl_pointer
 */
#define WL_POINTER_SET_CURSOR_SINCE_VERSION 1
/**
 * @ingroup iface_wl_pointer
 */
#define WL_POINTER_RELEASE_SINCE_VERSION 3

/** @ingroup iface_wl_pointer */
static inline void
wl_pointer_set_user_data(struct wl_pointer *wl_pointer, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_pointer, user_data);
}

/** @ingroup iface_wl_pointer */
static inline void *
wl_pointer_get_user_data(struct wl_pointer *wl_pointer)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_pointer);
}

static inline uint32_t
wl_pointer_get_version(struct wl_pointer *wl_pointer)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_pointer);
}

/** @ingroup iface_wl_pointer */
static inline void
wl_pointer_destroy(struct wl_pointer *wl_pointer)
{
	wl_proxy_destroy((struct wl_proxy *) wl_pointer);
}

/**
 * @ingroup iface_wl_pointer
 *
 * Set the pointer surface, i.e., the surface that contains the
 * pointer image (cursor). This request gives the surface the role
 * of a cursor. If the surface already has another role, it raises
 * a protocol error.
 *
 * The cursor actually changes only if the pointer
 * focus for this device is one of the requesting client's surfaces
 * or the surface parameter is the current pointer surface. If
 * there was a previous surface set with this request it is
 * replaced. If surface is NULL, the pointer image is hidden.
 *
 * The parameters hotspot_x and hotspot_y define the position of
 * the pointer surface relative to the pointer location. Its
 * top-left corner is always at (x, y) - (hotspot_x, hotspot_y),
 * where (x, y) are the coordinates of the pointer location, in
 * surface-local coordinates.
 *
 * On surface.attach requests to the pointer surface, hotspot_x
 * and hotspot_y are decremented by the x and y parameters
 * passed to the request. Attach must be confirmed by
 * wl_surface.commit as usual.
 *
 * The hotspot can also be updated by passing the currently set
 * pointer surface to this request with new values for hotspot_x
 * and hotspot_y.
 *
 * The current and pending input regions of the wl_surface are
 * cleared, and wl_surface.set_input_region is ignored until the
 * wl_surface is no longer used as the cursor. When the use as a
 * cursor ends, the current and pending input regions become
 * undefined, and the wl_surface is unmapped.
 */
static inline void
wl_pointer_set_cursor(struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface, int32_t hotspot_x, int32_t hotspot_y)
{
	wl_proxy_marshal((struct wl_proxy *) wl_pointer,
			 WL_POINTER_SET_CURSOR, serial, surface, hotspot_x, hotspot_y);
}

/**
 * @ingroup iface_wl_pointer
 *
 * Using this request a client can tell the server that it is not going to
 * use the pointer object anymore.
 *
 * This request destroys the pointer proxy object, so clients must not call
 * wl_pointer_destroy() after using this request.
 */
static inline void
wl_pointer_release(struct wl_pointer *wl_pointer)
{
	wl_proxy_marshal((struct wl_proxy *) wl_pointer,
			 WL_POINTER_RELEASE);

	wl_proxy_destroy((struct wl_proxy *) wl_pointer);
}

#ifndef WL_KEYBOARD_KEYMAP_FORMAT_ENUM
#define WL_KEYBOARD_KEYMAP_FORMAT_ENUM
/**
 * @ingroup iface_wl_keyboard
 * keyboard mapping format
 *
 * This specifies the format of the keymap provided to the
 * client with the wl_keyboard.keymap event.
 */
enum wl_keyboard_keymap_format {
	/**
	 * no keymap; client must understand how to interpret the raw keycode
	 */
	WL_KEYBOARD_KEYMAP_FORMAT_NO_KEYMAP = 0,
	/**
	 * libxkbcommon compatible; to determine the xkb keycode, clients must add 8 to the key event keycode
	 */
	WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1 = 1,
};
#endif /* WL_KEYBOARD_KEYMAP_FORMAT_ENUM */

#ifndef WL_KEYBOARD_KEY_STATE_ENUM
#define WL_KEYBOARD_KEY_STATE_ENUM
/**
 * @ingroup iface_wl_keyboard
 * physical key state
 *
 * Describes the physical state of a key that produced the key event.
 */
enum wl_keyboard_key_state {
	/**
	 * key is not pressed
	 */
	WL_KEYBOARD_KEY_STATE_RELEASED = 0,
	/**
	 * key is pressed
	 */
	WL_KEYBOARD_KEY_STATE_PRESSED = 1,
};
#endif /* WL_KEYBOARD_KEY_STATE_ENUM */

/**
 * @ingroup iface_wl_keyboard
 * @struct wl_keyboard_listener
 */
struct wl_keyboard_listener {
	/**
	 * keyboard mapping
	 *
	 * This event provides a file descriptor to the client which can
	 * be memory-mapped to provide a keyboard mapping description.
	 * @param format keymap format
	 * @param fd keymap file descriptor
	 * @param size keymap size, in bytes
	 */
	void (*keymap)(void *data,
		       struct wl_keyboard *wl_keyboard,
		       uint32_t format,
		       int32_t fd,
		       uint32_t size);
	/**
	 * enter event
	 *
	 * Notification that this seat's keyboard focus is on a certain
	 * surface.
	 * @param serial serial number of the enter event
	 * @param surface surface gaining keyboard focus
	 * @param keys the currently pressed keys
	 */
	void (*enter)(void *data,
		      struct wl_keyboard *wl_keyboard,
		      uint32_t serial,
		      struct wl_surface *surface,
		      struct wl_array *keys);
	/**
	 * leave event
	 *
	 * Notification that this seat's keyboard focus is no longer on a
	 * certain surface.
	 *
	 * The leave notification is sent before the enter notification for
	 * the new focus.
	 * @param serial serial number of the leave event
	 * @param surface surface that lost keyboard focus
	 */
	void (*leave)(void *data,
		      struct wl_keyboard *wl_keyboard,
		      uint32_t serial,
		      struct wl_surface *surface);
	/**
	 * key event
	 *
	 * A key was pressed or released. The time argument is a
	 * timestamp with millisecond granularity, with an undefined base.
	 * @param serial serial number of the key event
	 * @param time timestamp with millisecond granularity
	 * @param key key that produced the event
	 * @param state physical state of the key
	 */
	void (*key)(void *data,
		    struct wl_keyboard *wl_keyboard,
		    uint32_t serial,
		    uint32_t time,
		    uint32_t key,
		    uint32_t state);
	/**
	 * modifier and group state
	 *
	 * Notifies clients that the modifier and/or group state has
	 * changed, and it should update its local state.
	 * @param serial serial number of the modifiers event
	 * @param mods_depressed depressed modifiers
	 * @param mods_latched latched modifiers
	 * @param mods_locked locked modifiers
	 * @param group keyboard layout
	 */
	void (*modifiers)(void *data,
			  struct wl_keyboard *wl_keyboard,
			  uint32_t serial,
			  uint32_t mods_depressed,
			  uint32_t mods_latched,
			  uint32_t mods_locked,
			  uint32_t group);
	/**
	 * repeat rate and delay
	 *
	 * Informs the client about the keyboard's repeat rate and delay.
	 *
	 * This event is sent as soon as the wl_keyboard object has been
	 * created, and is guaranteed to be received by the client before
	 * any key press event.
	 *
	 * Negative values for either rate or delay are illegal. A rate of
	 * zero will disable any repeating (regardless of the value of
	 * delay).
	 *
	 * This event can be sent later on as well with a new value if
	 * necessary, so clients should continue listening for the event
	 * past the creation of wl_keyboard.
	 * @param rate the rate of repeating keys in characters per second
	 * @param delay delay in milliseconds since key down until repeating starts
	 * @since 4
	 */
	void (*repeat_info)(void *data,
			    struct wl_keyboard *wl_keyboard,
			    int32_t rate,
			    int32_t delay);
};

/**
 * @ingroup iface_wl_keyboard
 */
static inline int
wl_keyboard_add_listener(struct wl_keyboard *wl_keyboard,
			 const struct wl_keyboard_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_keyboard,
				     (void (**)(void)) listener, data);
}

#define WL_KEYBOARD_RELEASE 0

/**
 * @ingroup iface_wl_keyboard
 */
#define WL_KEYBOARD_KEYMAP_SINCE_VERSION 1
/**
 * @ingroup iface_wl_keyboard
 */
#define WL_KEYBOARD_ENTER_SINCE_VERSION 1
/**
 * @ingroup iface_wl_keyboard
 */
#define WL_KEYBOARD_LEAVE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_keyboard
 */
#define WL_KEYBOARD_KEY_SINCE_VERSION 1
/**
 * @ingroup iface_wl_keyboard
 */
#define WL_KEYBOARD_MODIFIERS_SINCE_VERSION 1
/**
 * @ingroup iface_wl_keyboard
 */
#define WL_KEYBOARD_REPEAT_INFO_SINCE_VERSION 4

/**
 * @ingroup iface_wl_keyboard
 */
#define WL_KEYBOARD_RELEASE_SINCE_VERSION 3

/** @ingroup iface_wl_keyboard */
static inline void
wl_keyboard_set_user_data(struct wl_keyboard *wl_keyboard, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_keyboard, user_data);
}

/** @ingroup iface_wl_keyboard */
static inline void *
wl_keyboard_get_user_data(struct wl_keyboard *wl_keyboard)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_keyboard);
}

static inline uint32_t
wl_keyboard_get_version(struct wl_keyboard *wl_keyboard)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_keyboard);
}

/** @ingroup iface_wl_keyboard */
static inline void
wl_keyboard_destroy(struct wl_keyboard *wl_keyboard)
{
	wl_proxy_destroy((struct wl_proxy *) wl_keyboard);
}

/**
 * @ingroup iface_wl_keyboard
 */
static inline void
wl_keyboard_release(struct wl_keyboard *wl_keyboard)
{
	wl_proxy_marshal((struct wl_proxy *) wl_keyboard,
			 WL_KEYBOARD_RELEASE);

	wl_proxy_destroy((struct wl_proxy *) wl_keyboard);
}

/**
 * @ingroup iface_wl_touch
 * @struct wl_touch_listener
 */
struct wl_touch_listener {
	/**
	 * touch down event and beginning of a touch sequence
	 *
	 * A new touch point has appeared on the surface. This touch
	 * point is assigned a unique ID. Future events from this touch
	 * point reference this ID. The ID ceases to be valid after a touch
	 * up event and may be reused in the future.
	 * @param serial serial number of the touch down event
	 * @param time timestamp with millisecond granularity
	 * @param surface surface touched
	 * @param id the unique ID of this touch point
	 * @param x surface-local x coordinate
	 * @param y surface-local y coordinate
	 */
	void (*down)(void *data,
		     struct wl_touch *wl_touch,
		     uint32_t serial,
		     uint32_t time,
		     struct wl_surface *surface,
		     int32_t id,
		     wl_fixed_t x,
		     wl_fixed_t y);
	/**
	 * end of a touch event sequence
	 *
	 * The touch point has disappeared. No further events will be
	 * sent for this touch point and the touch point's ID is released
	 * and may be reused in a future touch down event.
	 * @param serial serial number of the touch up event
	 * @param time timestamp with millisecond granularity
	 * @param id the unique ID of this touch point
	 */
	void (*up)(void *data,
		   struct wl_touch *wl_touch,
		   uint32_t serial,
		   uint32_t time,
		   int32_t id);
	/**
	 * update of touch point coordinates
	 *
	 * A touch point has changed coordinates.
	 * @param time timestamp with millisecond granularity
	 * @param id the unique ID of this touch point
	 * @param x surface-local x coordinate
	 * @param y surface-local y coordinate
	 */
	void (*motion)(void *data,
		       struct wl_touch *wl_touch,
		       uint32_t time,
		       int32_t id,
		       wl_fixed_t x,
		       wl_fixed_t y);
	/**
	 * end of touch frame event
	 *
	 * Indicates the end of a set of events that logically belong
	 * together. A client is expected to accumulate the data in all
	 * events within the frame before proceeding.
	 *
	 * A wl_touch.frame terminates at least one event but otherwise no
	 * guarantee is provided about the set of events within a frame. A
	 * client must assume that any state not updated in a frame is
	 * unchanged from the previously known state.
	 */
	void (*frame)(void *data,
		      struct wl_touch *wl_touch);
	/**
	 * touch session cancelled
	 *
	 * Sent if the compositor decides the touch stream is a global
	 * gesture. No further events are sent to the clients from that
	 * particular gesture. Touch cancellation applies to all touch
	 * points currently active on this client's surface. The client is
	 * responsible for finalizing the touch points, future touch points
	 * on this surface may reuse the touch point ID.
	 */
	void (*cancel)(void *data,
		       struct wl_touch *wl_touch);
	/**
	 * update shape of touch point
	 *
	 * Sent when a touchpoint has changed its shape.
	 *
	 * This event does not occur on its own. It is sent before a
	 * wl_touch.frame event and carries the new shape information for
	 * any previously reported, or new touch points of that frame.
	 *
	 * Other events describing the touch point such as wl_touch.down,
	 * wl_touch.motion or wl_touch.orientation may be sent within the
	 * same wl_touch.frame. A client should treat these events as a
	 * single logical touch point update. The order of wl_touch.shape,
	 * wl_touch.orientation and wl_touch.motion is not guaranteed. A
	 * wl_touch.down event is guaranteed to occur before the first
	 * wl_touch.shape event for this touch ID but both events may occur
	 * within the same wl_touch.frame.
	 *
	 * A touchpoint shape is approximated by an ellipse through the
	 * major and minor axis length. The major axis length describes the
	 * longer diameter of the ellipse, while the minor axis length
	 * describes the shorter diameter. Major and minor are orthogonal
	 * and both are specified in surface-local coordinates. The center
	 * of the ellipse is always at the touchpoint location as reported
	 * by wl_touch.down or wl_touch.move.
	 *
	 * This event is only sent by the compositor if the touch device
	 * supports shape reports. The client has to make reasonable
	 * assumptions about the shape if it did not receive this event.
	 * @param id the unique ID of this touch point
	 * @param major length of the major axis in surface-local coordinates
	 * @param minor length of the minor axis in surface-local coordinates
	 * @since 6
	 */
	void (*shape)(void *data,
		      struct wl_touch *wl_touch,
		      int32_t id,
		      wl_fixed_t major,
		      wl_fixed_t minor);
	/**
	 * update orientation of touch point
	 *
	 * Sent when a touchpoint has changed its orientation.
	 *
	 * This event does not occur on its own. It is sent before a
	 * wl_touch.frame event and carries the new shape information for
	 * any previously reported, or new touch points of that frame.
	 *
	 * Other events describing the touch point such as wl_touch.down,
	 * wl_touch.motion or wl_touch.shape may be sent within the same
	 * wl_touch.frame. A client should treat these events as a single
	 * logical touch point update. The order of wl_touch.shape,
	 * wl_touch.orientation and wl_touch.motion is not guaranteed. A
	 * wl_touch.down event is guaranteed to occur before the first
	 * wl_touch.orientation event for this touch ID but both events may
	 * occur within the same wl_touch.frame.
	 *
	 * The orientation describes the clockwise angle of a touchpoint's
	 * major axis to the positive surface y-axis and is normalized to
	 * the -180 to +180 degree range. The granularity of orientation
	 * depends on the touch device, some devices only support binary
	 * rotation values between 0 and 90 degrees.
	 *
	 * This event is only sent by the compositor if the touch device
	 * supports orientation reports.
	 * @param id the unique ID of this touch point
	 * @param orientation angle between major axis and positive surface y-axis in degrees
	 * @since 6
	 */
	void (*orientation)(void *data,
			    struct wl_touch *wl_touch,
			    int32_t id,
			    wl_fixed_t orientation);
};

/**
 * @ingroup iface_wl_touch
 */
static inline int
wl_touch_add_listener(struct wl_touch *wl_touch,
		      const struct wl_touch_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_touch,
				     (void (**)(void)) listener, data);
}

#define WL_TOUCH_RELEASE 0

/**
 * @ingroup iface_wl_touch
 */
#define WL_TOUCH_DOWN_SINCE_VERSION 1
/**
 * @ingroup iface_wl_touch
 */
#define WL_TOUCH_UP_SINCE_VERSION 1
/**
 * @ingroup iface_wl_touch
 */
#define WL_TOUCH_MOTION_SINCE_VERSION 1
/**
 * @ingroup iface_wl_touch
 */
#define WL_TOUCH_FRAME_SINCE_VERSION 1
/**
 * @ingroup iface_wl_touch
 */
#define WL_TOUCH_CANCEL_SINCE_VERSION 1
/**
 * @ingroup iface_wl_touch
 */
#define WL_TOUCH_SHAPE_SINCE_VERSION 6
/**
 * @ingroup iface_wl_touch
 */
#define WL_TOUCH_ORIENTATION_SINCE_VERSION 6

/**
 * @ingroup iface_wl_touch
 */
#define WL_TOUCH_RELEASE_SINCE_VERSION 3

/** @ingroup iface_wl_touch */
static inline void
wl_touch_set_user_data(struct wl_touch *wl_touch, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_touch, user_data);
}

/** @ingroup iface_wl_touch */
static inline void *
wl_touch_get_user_data(struct wl_touch *wl_touch)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_touch);
}

static inline uint32_t
wl_touch_get_version(struct wl_touch *wl_touch)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_touch);
}

/** @ingroup iface_wl_touch */
static inline void
wl_touch_destroy(struct wl_touch *wl_touch)
{
	wl_proxy_destroy((struct wl_proxy *) wl_touch);
}

/**
 * @ingroup iface_wl_touch
 */
static inline void
wl_touch_release(struct wl_touch *wl_touch)
{
	wl_proxy_marshal((struct wl_proxy *) wl_touch,
			 WL_TOUCH_RELEASE);

	wl_proxy_destroy((struct wl_proxy *) wl_touch);
}

#ifndef WL_OUTPUT_SUBPIXEL_ENUM
#define WL_OUTPUT_SUBPIXEL_ENUM
/**
 * @ingroup iface_wl_output
 * subpixel geometry information
 *
 * This enumeration describes how the physical
 * pixels on an output are laid out.
 */
enum wl_output_subpixel {
	/**
	 * unknown geometry
	 */
	WL_OUTPUT_SUBPIXEL_UNKNOWN = 0,
	/**
	 * no geometry
	 */
	WL_OUTPUT_SUBPIXEL_NONE = 1,
	/**
	 * horizontal RGB
	 */
	WL_OUTPUT_SUBPIXEL_HORIZONTAL_RGB = 2,
	/**
	 * horizontal BGR
	 */
	WL_OUTPUT_SUBPIXEL_HORIZONTAL_BGR = 3,
	/**
	 * vertical RGB
	 */
	WL_OUTPUT_SUBPIXEL_VERTICAL_RGB = 4,
	/**
	 * vertical BGR
	 */
	WL_OUTPUT_SUBPIXEL_VERTICAL_BGR = 5,
};
#endif /* WL_OUTPUT_SUBPIXEL_ENUM */

#ifndef WL_OUTPUT_TRANSFORM_ENUM
#define WL_OUTPUT_TRANSFORM_ENUM
/**
 * @ingroup iface_wl_output
 * transform from framebuffer to output
 *
 * This describes the transform that a compositor will apply to a
 * surface to compensate for the rotation or mirroring of an
 * output device.
 *
 * The flipped values correspond to an initial flip around a
 * vertical axis followed by rotation.
 *
 * The purpose is mainly to allow clients to render accordingly and
 * tell the compositor, so that for fullscreen surfaces, the
 * compositor will still be able to scan out directly from client
 * surfaces.
 */
enum wl_output_transform {
	/**
	 * no transform
	 */
	WL_OUTPUT_TRANSFORM_NORMAL = 0,
	/**
	 * 90 degrees counter-clockwise
	 */
	WL_OUTPUT_TRANSFORM_90 = 1,
	/**
	 * 180 degrees counter-clockwise
	 */
	WL_OUTPUT_TRANSFORM_180 = 2,
	/**
	 * 270 degrees counter-clockwise
	 */
	WL_OUTPUT_TRANSFORM_270 = 3,
	/**
	 * 180 degree flip around a vertical axis
	 */
	WL_OUTPUT_TRANSFORM_FLIPPED = 4,
	/**
	 * flip and rotate 90 degrees counter-clockwise
	 */
	WL_OUTPUT_TRANSFORM_FLIPPED_90 = 5,
	/**
	 * flip and rotate 180 degrees counter-clockwise
	 */
	WL_OUTPUT_TRANSFORM_FLIPPED_180 = 6,
	/**
	 * flip and rotate 270 degrees counter-clockwise
	 */
	WL_OUTPUT_TRANSFORM_FLIPPED_270 = 7,
};
#endif /* WL_OUTPUT_TRANSFORM_ENUM */

#ifndef WL_OUTPUT_MODE_ENUM
#define WL_OUTPUT_MODE_ENUM
/**
 * @ingroup iface_wl_output
 * mode information
 *
 * These flags describe properties of an output mode.
 * They are used in the flags bitfield of the mode event.
 */
enum wl_output_mode {
	/**
	 * indicates this is the current mode
	 */
	WL_OUTPUT_MODE_CURRENT = 0x1,
	/**
	 * indicates this is the preferred mode
	 */
	WL_OUTPUT_MODE_PREFERRED = 0x2,
};
#endif /* WL_OUTPUT_MODE_ENUM */

/**
 * @ingroup iface_wl_output
 * @struct wl_output_listener
 */
struct wl_output_listener {
	/**
	 * properties of the output
	 *
	 * The geometry event describes geometric properties of the
	 * output. The event is sent when binding to the output object and
	 * whenever any of the properties change.
	 * @param x x position within the global compositor space
	 * @param y y position within the global compositor space
	 * @param physical_width width in millimeters of the output
	 * @param physical_height height in millimeters of the output
	 * @param subpixel subpixel orientation of the output
	 * @param make textual description of the manufacturer
	 * @param model textual description of the model
	 * @param transform transform that maps framebuffer to output
	 */
	void (*geometry)(void *data,
			 struct wl_output *wl_output,
			 int32_t x,
			 int32_t y,
			 int32_t physical_width,
			 int32_t physical_height,
			 int32_t subpixel,
			 const char *make,
			 const char *model,
			 int32_t transform);
	/**
	 * advertise available modes for the output
	 *
	 * The mode event describes an available mode for the output.
	 *
	 * The event is sent when binding to the output object and there
	 * will always be one mode, the current mode. The event is sent
	 * again if an output changes mode, for the mode that is now
	 * current. In other words, the current mode is always the last
	 * mode that was received with the current flag set.
	 *
	 * The size of a mode is given in physical hardware units of the
	 * output device. This is not necessarily the same as the output
	 * size in the global compositor space. For instance, the output
	 * may be scaled, as described in wl_output.scale, or transformed,
	 * as described in wl_output.transform.
	 * @param flags bitfield of mode flags
	 * @param width width of the mode in hardware units
	 * @param height height of the mode in hardware units
	 * @param refresh vertical refresh rate in mHz
	 */
	void (*mode)(void *data,
		     struct wl_output *wl_output,
		     uint32_t flags,
		     int32_t width,
		     int32_t height,
		     int32_t refresh);
	/**
	 * sent all information about output
	 *
	 * This event is sent after all other properties have been sent
	 * after binding to the output object and after any other property
	 * changes done after that. This allows changes to the output
	 * properties to be seen as atomic, even if they happen via
	 * multiple events.
	 * @since 2
	 */
	void (*done)(void *data,
		     struct wl_output *wl_output);
	/**
	 * output scaling properties
	 *
	 * This event contains scaling geometry information that is not
	 * in the geometry event. It may be sent after binding the output
	 * object or if the output scale changes later. If it is not sent,
	 * the client should assume a scale of 1.
	 *
	 * A scale larger than 1 means that the compositor will
	 * automatically scale surface buffers by this amount when
	 * rendering. This is used for very high resolution displays where
	 * applications rendering at the native resolution would be too
	 * small to be legible.
	 *
	 * It is intended that scaling aware clients track the current
	 * output of a surface, and if it is on a scaled output it should
	 * use wl_surface.set_buffer_scale with the scale of the output.
	 * That way the compositor can avoid scaling the surface, and the
	 * client can supply a higher detail image.
	 * @param factor scaling factor of output
	 * @since 2
	 */
	void (*scale)(void *data,
		      struct wl_output *wl_output,
		      int32_t factor);
};

/**
 * @ingroup iface_wl_output
 */
static inline int
wl_output_add_listener(struct wl_output *wl_output,
		       const struct wl_output_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_output,
				     (void (**)(void)) listener, data);
}

#define WL_OUTPUT_RELEASE 0

/**
 * @ingroup iface_wl_output
 */
#define WL_OUTPUT_GEOMETRY_SINCE_VERSION 1
/**
 * @ingroup iface_wl_output
 */
#define WL_OUTPUT_MODE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_output
 */
#define WL_OUTPUT_DONE_SINCE_VERSION 2
/**
 * @ingroup iface_wl_output
 */
#define WL_OUTPUT_SCALE_SINCE_VERSION 2

/**
 * @ingroup iface_wl_output
 */
#define WL_OUTPUT_RELEASE_SINCE_VERSION 3

/** @ingroup iface_wl_output */
static inline void
wl_output_set_user_data(struct wl_output *wl_output, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_output, user_data);
}

/** @ingroup iface_wl_output */
static inline void *
wl_output_get_user_data(struct wl_output *wl_output)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_output);
}

static inline uint32_t
wl_output_get_version(struct wl_output *wl_output)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_output);
}

/** @ingroup iface_wl_output */
static inline void
wl_output_destroy(struct wl_output *wl_output)
{
	wl_proxy_destroy((struct wl_proxy *) wl_output);
}

/**
 * @ingroup iface_wl_output
 *
 * Using this request a client can tell the server that it is not going to
 * use the output object anymore.
 */
static inline void
wl_output_release(struct wl_output *wl_output)
{
	wl_proxy_marshal((struct wl_proxy *) wl_output,
			 WL_OUTPUT_RELEASE);

	wl_proxy_destroy((struct wl_proxy *) wl_output);
}

#define WL_REGION_DESTROY 0
#define WL_REGION_ADD 1
#define WL_REGION_SUBTRACT 2


/**
 * @ingroup iface_wl_region
 */
#define WL_REGION_DESTROY_SINCE_VERSION 1
/**
 * @ingroup iface_wl_region
 */
#define WL_REGION_ADD_SINCE_VERSION 1
/**
 * @ingroup iface_wl_region
 */
#define WL_REGION_SUBTRACT_SINCE_VERSION 1

/** @ingroup iface_wl_region */
static inline void
wl_region_set_user_data(struct wl_region *wl_region, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_region, user_data);
}

/** @ingroup iface_wl_region */
static inline void *
wl_region_get_user_data(struct wl_region *wl_region)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_region);
}

static inline uint32_t
wl_region_get_version(struct wl_region *wl_region)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_region);
}

/**
 * @ingroup iface_wl_region
 *
 * Destroy the region.  This will invalidate the object ID.
 */
static inline void
wl_region_destroy(struct wl_region *wl_region)
{
	wl_proxy_marshal((struct wl_proxy *) wl_region,
			 WL_REGION_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_region);
}

/**
 * @ingroup iface_wl_region
 *
 * Add the specified rectangle to the region.
 */
static inline void
wl_region_add(struct wl_region *wl_region, int32_t x, int32_t y, int32_t width, int32_t height)
{
	wl_proxy_marshal((struct wl_proxy *) wl_region,
			 WL_REGION_ADD, x, y, width, height);
}

/**
 * @ingroup iface_wl_region
 *
 * Subtract the specified rectangle from the region.
 */
static inline void
wl_region_subtract(struct wl_region *wl_region, int32_t x, int32_t y, int32_t width, int32_t height)
{
	wl_proxy_marshal((struct wl_proxy *) wl_region,
			 WL_REGION_SUBTRACT, x, y, width, height);
}

#ifndef WL_SUBCOMPOSITOR_ERROR_ENUM
#define WL_SUBCOMPOSITOR_ERROR_ENUM
enum wl_subcompositor_error {
	/**
	 * the to-be sub-surface is invalid
	 */
	WL_SUBCOMPOSITOR_ERROR_BAD_SURFACE = 0,
};
#endif /* WL_SUBCOMPOSITOR_ERROR_ENUM */

#define WL_SUBCOMPOSITOR_DESTROY 0
#define WL_SUBCOMPOSITOR_GET_SUBSURFACE 1


/**
 * @ingroup iface_wl_subcompositor
 */
#define WL_SUBCOMPOSITOR_DESTROY_SINCE_VERSION 1
/**
 * @ingroup iface_wl_subcompositor
 */
#define WL_SUBCOMPOSITOR_GET_SUBSURFACE_SINCE_VERSION 1

/** @ingroup iface_wl_subcompositor */
static inline void
wl_subcompositor_set_user_data(struct wl_subcompositor *wl_subcompositor, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_subcompositor, user_data);
}

/** @ingroup iface_wl_subcompositor */
static inline void *
wl_subcompositor_get_user_data(struct wl_subcompositor *wl_subcompositor)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_subcompositor);
}

static inline uint32_t
wl_subcompositor_get_version(struct wl_subcompositor *wl_subcompositor)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_subcompositor);
}

/**
 * @ingroup iface_wl_subcompositor
 *
 * Informs the server that the client will not be using this
 * protocol object anymore. This does not affect any other
 * objects, wl_subsurface objects included.
 */
static inline void
wl_subcompositor_destroy(struct wl_subcompositor *wl_subcompositor)
{
	wl_proxy_marshal((struct wl_proxy *) wl_subcompositor,
			 WL_SUBCOMPOSITOR_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_subcompositor);
}

/**
 * @ingroup iface_wl_subcompositor
 *
 * Create a sub-surface interface for the given surface, and
 * associate it with the given parent surface. This turns a
 * plain wl_surface into a sub-surface.
 *
 * The to-be sub-surface must not already have another role, and it
 * must not have an existing wl_subsurface object. Otherwise a protocol
 * error is raised.
 */
static inline struct wl_subsurface *
wl_subcompositor_get_subsurface(struct wl_subcompositor *wl_subcompositor, struct wl_surface *surface, struct wl_surface *parent)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) wl_subcompositor,
			 WL_SUBCOMPOSITOR_GET_SUBSURFACE, &wl_subsurface_interface, NULL, surface, parent);

	return (struct wl_subsurface *) id;
}

#ifndef WL_SUBSURFACE_ERROR_ENUM
#define WL_SUBSURFACE_ERROR_ENUM
enum wl_subsurface_error {
	/**
	 * wl_surface is not a sibling or the parent
	 */
	WL_SUBSURFACE_ERROR_BAD_SURFACE = 0,
};
#endif /* WL_SUBSURFACE_ERROR_ENUM */

#define WL_SUBSURFACE_DESTROY 0
#define WL_SUBSURFACE_SET_POSITION 1
#define WL_SUBSURFACE_PLACE_ABOVE 2
#define WL_SUBSURFACE_PLACE_BELOW 3
#define WL_SUBSURFACE_SET_SYNC 4
#define WL_SUBSURFACE_SET_DESYNC 5


/**
 * @ingroup iface_wl_subsurface
 */
#define WL_SUBSURFACE_DESTROY_SINCE_VERSION 1
/**
 * @ingroup iface_wl_subsurface
 */
#define WL_SUBSURFACE_SET_POSITION_SINCE_VERSION 1
/**
 * @ingroup iface_wl_subsurface
 */
#define WL_SUBSURFACE_PLACE_ABOVE_SINCE_VERSION 1
/**
 * @ingroup iface_wl_subsurface
 */
#define WL_SUBSURFACE_PLACE_BELOW_SINCE_VERSION 1
/**
 * @ingroup iface_wl_subsurface
 */
#define WL_SUBSURFACE_SET_SYNC_SINCE_VERSION 1
/**
 * @ingroup iface_wl_subsurface
 */
#define WL_SUBSURFACE_SET_DESYNC_SINCE_VERSION 1

/** @ingroup iface_wl_subsurface */
static inline void
wl_subsurface_set_user_data(struct wl_subsurface *wl_subsurface, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_subsurface, user_data);
}

/** @ingroup iface_wl_subsurface */
static inline void *
wl_subsurface_get_user_data(struct wl_subsurface *wl_subsurface)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_subsurface);
}

static inline uint32_t
wl_subsurface_get_version(struct wl_subsurface *wl_subsurface)
{
	return wl_proxy_get_version((struct wl_proxy *) wl_subsurface);
}

/**
 * @ingroup iface_wl_subsurface
 *
 * The sub-surface interface is removed from the wl_surface object
 * that was turned into a sub-surface with a
 * wl_subcompositor.get_subsurface request. The wl_surface's association
 * to the parent is deleted, and the wl_surface loses its role as
 * a sub-surface. The wl_surface is unmapped.
 */
static inline void
wl_subsurface_destroy(struct wl_subsurface *wl_subsurface)
{
	wl_proxy_marshal((struct wl_proxy *) wl_subsurface,
			 WL_SUBSURFACE_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_subsurface);
}

/**
 * @ingroup iface_wl_subsurface
 *
 * This schedules a sub-surface position change.
 * The sub-surface will be moved so that its origin (top left
 * corner pixel) will be at the location x, y of the parent surface
 * coordinate system. The coordinates are not restricted to the parent
 * surface area. Negative values are allowed.
 *
 * The scheduled coordinates will take effect whenever the state of the
 * parent surface is applied. When this happens depends on whether the
 * parent surface is in synchronized mode or not. See
 * wl_subsurface.set_sync and wl_subsurface.set_desync for details.
 *
 * If more than one set_position request is invoked by the client before
 * the commit of the parent surface, the position of a new request always
 * replaces the scheduled position from any previous request.
 *
 * The initial position is 0, 0.
 */
static inline void
wl_subsurface_set_position(struct wl_subsurface *wl_subsurface, int32_t x, int32_t y)
{
	wl_proxy_marshal((struct wl_proxy *) wl_subsurface,
			 WL_SUBSURFACE_SET_POSITION, x, y);
}

/**
 * @ingroup iface_wl_subsurface
 *
 * This sub-surface is taken from the stack, and put back just
 * above the reference surface, changing the z-order of the sub-surfaces.
 * The reference surface must be one of the sibling surfaces, or the
 * parent surface. Using any other surface, including this sub-surface,
 * will cause a protocol error.
 *
 * The z-order is double-buffered. Requests are handled in order and
 * applied immediately to a pending state. The final pending state is
 * copied to the active state the next time the state of the parent
 * surface is applied. When this happens depends on whether the parent
 * surface is in synchronized mode or not. See wl_subsurface.set_sync and
 * wl_subsurface.set_desync for details.
 *
 * A new sub-surface is initially added as the top-most in the stack
 * of its siblings and parent.
 */
static inline void
wl_subsurface_place_above(struct wl_subsurface *wl_subsurface, struct wl_surface *sibling)
{
	wl_proxy_marshal((struct wl_proxy *) wl_subsurface,
			 WL_SUBSURFACE_PLACE_ABOVE, sibling);
}

/**
 * @ingroup iface_wl_subsurface
 *
 * The sub-surface is placed just below the reference surface.
 * See wl_subsurface.place_above.
 */
static inline void
wl_subsurface_place_below(struct wl_subsurface *wl_subsurface, struct wl_surface *sibling)
{
	wl_proxy_marshal((struct wl_proxy *) wl_subsurface,
			 WL_SUBSURFACE_PLACE_BELOW, sibling);
}

/**
 * @ingroup iface_wl_subsurface
 *
 * Change the commit behaviour of the sub-surface to synchronized
 * mode, also described as the parent dependent mode.
 *
 * In synchronized mode, wl_surface.commit on a sub-surface will
 * accumulate the committed state in a cache, but the state will
 * not be applied and hence will not change the compositor output.
 * The cached state is applied to the sub-surface immediately after
 * the parent surface's state is applied. This ensures atomic
 * updates of the parent and all its synchronized sub-surfaces.
 * Applying the cached state will invalidate the cache, so further
 * parent surface commits do not (re-)apply old state.
 *
 * See wl_subsurface for the recursive effect of this mode.
 */
static inline void
wl_subsurface_set_sync(struct wl_subsurface *wl_subsurface)
{
	wl_proxy_marshal((struct wl_proxy *) wl_subsurface,
			 WL_SUBSURFACE_SET_SYNC);
}

/**
 * @ingroup iface_wl_subsurface
 *
 * Change the commit behaviour of the sub-surface to desynchronized
 * mode, also described as independent or freely running mode.
 *
 * In desynchronized mode, wl_surface.commit on a sub-surface will
 * apply the pending state directly, without caching, as happens
 * normally with a wl_surface. Calling wl_surface.commit on the
 * parent surface has no effect on the sub-surface's wl_surface
 * state. This mode allows a sub-surface to be updated on its own.
 *
 * If cached state exists when wl_surface.commit is called in
 * desynchronized mode, the pending state is added to the cached
 * state, and applied as a whole. This invalidates the cache.
 *
 * Note: even if a sub-surface is set to desynchronized, a parent
 * sub-surface may override it to behave as synchronized. For details,
 * see wl_subsurface.
 *
 * If a surface's parent surface behaves as desynchronized, then
 * the cached state is applied on set_desync.
 */
static inline void
wl_subsurface_set_desync(struct wl_subsurface *wl_subsurface)
{
	wl_proxy_marshal((struct wl_proxy *) wl_subsurface,
			 WL_SUBSURFACE_SET_DESYNC);
}

#ifdef  __cplusplus
}
#endif

#endif
