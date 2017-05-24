import gudhi

alpha_complex = gudhi.AlphaComplex(off_file='../tore3D_300.off')
diag = alpha_complex.persistence()
gudhi.plot_persistence_diagram(diag)
