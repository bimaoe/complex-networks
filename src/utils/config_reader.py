"""This file generates networks with parameters specified in parameter_list_dict."""
import argparse

def parse(config_filename):
  config_file =open(config_filename, 'r')
  config = {}
  stack = []

  curr = config
  next = None
  for line in config_file:
    if not line:
      continue
    var = line.strip().split(' ')
    if var[0] == '#':
      continue
    elif var[0] == '+':
      stack.append(curr)
      curr = next
    elif var[0] == '-':
      curr = stack.pop()
    elif var[0] == 'single':
      curr[var[1]] = var[2]
    elif var[0] == 'list':
      curr[var[1]] = var[2].split('|')
    elif var[0] == 'dict':
      curr[var[1]] = {}
      next = curr[var[1]]
  return config

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-c', '--config_filename',
      help='File where the parameters are.', required=True)
  
  args = vars(parser.parse_args())
  config_filename = args['config_filename']
  
  config = parse(config_filename)
  print config

if __name__ == '__main__':
  main()
  