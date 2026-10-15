# Pixel Football

Original student project: https://github.com/gsilva00/LCOM-Project

Pinned revision: `4e793123ae3cd5ad486e1472fa715cff6567c66e`.

Student source and required art are preserved byte for byte except explicitly recorded corrections below. lcom.json and icon.png are collection additions. Unrelated reports, generated documentation, build products and Git history are omitted.

License: No license file was supplied by this upstream revision; source availability does not grant redistribution permission.

## Confirmed source correction

The original free_buffers() passed vm_map_phys() storage to free(), aborting even when selecting Quit from the original menu. The one-line SOURCE_PATCH.diff releases the mapping with vm_unmap_phys() instead. This changes resource cleanup only. Original source hashes remain in provenance.
