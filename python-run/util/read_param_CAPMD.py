# SAMoSA: Read parameters from a json file. Made for CAPMD
import json

class Param:
	def __init__(self,p):		

		# here is the labels we definitely need:
		self.N = p.N # Number of particles
		self.dt = p.dt # time step
		self.dumpname = 'ParticleData'  # root name of particle data csv files
		self.dump = {}
		self.dump['freq'] = p.output_time
		# Geometry: always a plane, either with or without periodic boundary conditions
		if p.bc_opt != 'periodic':
			self.periodic = False
			self.constraint = 'plane'
		else:
			self.periodic = True
			self.constraint = 'plane_periodic'

		self.lx = p.Lx
		self.ly = p.Ly
		# put into a box to be compatible with samos convention
		# Note height. Just important it's larger than the maximum distance between contacting cells
		# Else problems with the cell list later on.
		self.box = [p.Lx,p.Ly,5.0]
		
		# Potential parameters (will need to be modified for several stiffnesses)
		self.potential = 'soft_attractive' # our current potential
		self.pot_params = {}
		self.pot_params['k'] = p.pairstiff # stiffness
		
		self.pot_params['re_fact'] = [[att+1 for att in pair] for pair in p.pairatt]  # point at which the potential hits its minimum. We label that 1 + epsilon
		# alternative option when interactions are turned off
		self.potential = 'none'
		self.pot_params['k'] = 0.0

		# We are always using brownian dynamics 
		self.one_integrator = True
		self.integrator = 'brownian'
		self.seed = p.angseed # seed of orientational noise term
		self.mu = [1/z if z != 0 else 0 for z in p.zeta] # mobility (1/friction), usually 1 in simulation units
		self.v0 = p.factive # self-propulsion speed
		self.nu = [1/t if t != 0 else 0 for t in p.tau] # magnitude of orientational noise
		
		# We may want to add alignment at some point.
		# Currently none
		self.J = 0.0
		self.aligner = 'none'

		# Velocity alignment
		#self.aligner = 'ajpolar'
		#self.J = float(paramdict["zetaVelocity"])

		# number of types in the simulation (including boundary)
		self.ntypes = p.ntypes

		# For our current code
		self.npopulation = 1
		self.population = ['regular']
		self.pop_params=[{}]
		self.pop_params[0]['divrate'] =  p.divrate# division rate
		self.pop_params[0]['deathrate'] =  p.deathrate# division rate
		self.pop_params[0]['zmax'] = p.maxZ # max contact number (for division, can have one for death too)
		self.pop_params[0]['frequency'] = p.popdynfreq # after how many time steps we apply population in code

		# TODO: Dealing with different populations and their different division / death rates
		# self.npopulation = 2 # number of different groups of cells doing their own division / death dynamics (e.g. = 2 for p53 / not p53 cells)

		# give them names by hand, can be part of json file too
		#self.population=['regular','p53']
		if self.npopulation>1:
			print ("Number of populations: " + str(self.npopulation))
			self.pop_params=[{} for k in range(self.npopulation)]
			for k in range(self.npopulation):
				self.population.append(conf.key_words['population'][k].name)
				# self.pop_params[k]['divrate'] =  # division rate
				# self.pop_params[k]['deathrate'] =  # death rate
				# self.pop_params[k]['zmax'] =  # max contact number (for division, can have one for death too)
				# self.pop_params[k]['frequency'] =  # after how many time steps we apply population in code
				# We can add others if and when necessary
			print ("Populations: ")
			print (self.population)
			print ("Population parameters: ")
			print (self.pop_params)
